#include "disass.h"

const char *COMMAND[] = {
    "hlt",
    "out",
    "push",
    "add",
    "sub",
    "mul",
    "div",
    "sqrt",
    "sin",
    "cos",
    "in",
    "pop"
};

const char *REG[] = {
    "rax",
    "rbx",
    "rcx",
    "rdx"
};

const int COMMAND_CNT = 11;

int input_text (TEXT* data)
{
    my_assert (data != NULL);

    data->file_name_input = (const char *) "..\\include\\ass_output.txt";

    data->fp_input = fopen (data->file_name_input, "rb");

    if (data->fp_input == NULL)
    {
        return ERR_FOPEN;
    }

    data->size_file = get_file_size (data->fp_input);

    data->buf = (char *) calloc (data->size_file + 1, sizeof (char));
    my_assert (data->buf != NULL);

    size_t read_size = fread (data->buf, sizeof (char), data->size_file, data->fp_input);

    if (read_size != data->size_file)
    {
        return ERR_FREAD;
    }

    *(data->buf + data->size_file) = '\0';

    if (fclose (data->fp_input) != 0)
    {
        return ERR_FCLOSE;
    }

    int code_error = split_commands (data);

    return code_error;
}

int split_commands (TEXT *data)
{   
    my_assert (data != NULL);

    data->n_comms =  number_of_lines (data->buf, data->size_file);

    data->cmd = (COMMANDS *) calloc (data->n_comms, sizeof(COMMANDS));
    my_assert (data->cmd != NULL);

    char buf[256] = "";

    for (size_t id = 0; id < data->n_comms; id++)
    {
        int value = -1;

        if (sscanf (data->buf, "%d", &value) != 1)
        {
            return ERR_COMMAND;
        }

        sprintf (buf, "%d", value);
        data->buf = move_point (data, buf);   

        if (value == (PUSH + (1 << 5)))
        {
            data->cmd[id].command = PUSH;

            if (sscanf (data->buf, "%d", &data->cmd[id].reg) != 1)
            {
                return ERR_REG_PUSH;
            }

            if (data->cmd[id].reg < RAX || data->cmd[id].reg > RDX)
            {
                return ERR_REG_PUSH;
            }

            sprintf (buf, "%d", data->cmd[id].reg);
            data->buf = move_point (data, buf);  
        }
        else if (value == (PUSH + (1 << 4)))
        {
            data->cmd[id].command = PUSH;

            if (sscanf (data->buf, "%d", &data->cmd[id].argc) != 1)
            {
                return ERR_ARGC;
            }

            sprintf (buf, "%d", data->cmd[id].argc);
            data->buf = move_point (data, buf);
        }
        else if (value == (POP + (1 << 5)))
        {
            data->cmd[id].command = POP;

            if (sscanf (data->buf, "%d", &data->cmd[id].reg) != 1)
            {
                return ERR_REG_POP;
            }

            if (data->cmd[id].reg < RAX || data->cmd[id].reg > RDX)
            {
                return ERR_REG_POP;
            }

            sprintf (buf, "%d", data->cmd[id].reg);
            data->buf = move_point (data, buf);
        }
        else if (value >= HLT && value <= POP)
        {
            data->cmd[id].command = value;
        }
        else
        {
            return ERR_COMMAND;
        }
    }

    return ERR_NO;
}

void print_text (TEXT *data)
{
    my_assert (data != NULL);
    
    for (size_t id = 0; id < data->n_comms; id++)
    {
        if (data->cmd[id].command == PUSH)
        {
            if (data->cmd[id].reg != COMMANDS_DEFAULT)
            {
                fprintf (data->fp_print, "%s %s\n", COMMAND[data->cmd[id].command], REG[data->cmd[id].reg - 1]);
            }
            else
            {
                fprintf (data->fp_print, "%s %d\n", COMMAND[data->cmd[id].command], (data->cmd[id].argc));
            }
        }
        else if (data->cmd[id].command == POP)
        {   
            if (data->cmd[id].reg != COMMANDS_DEFAULT)
            {
                fprintf (data->fp_print, "%s %s\n", COMMAND[data->cmd[id].command], REG[data->cmd[id].reg - 1]);
            }
            else
            {
                fprintf (data->fp_print, "%s\n", COMMAND[data->cmd[id].command]);
            }
        }
        else if (data->cmd[id].command == COMMANDS_DEFAULT)
        {
            fprintf (data->fp_print, "%s", COMMAND[data->cmd[id].command]);
        }
        else
        {
            fprintf (data->fp_print, "%s\n", COMMAND[data->cmd[id].command]);
        }
    }
}
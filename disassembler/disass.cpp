#include "disass.h"

static size_t number_of_lines (const char *data, const size_t size);

static char *move_point (TEXT *data, const char *buf);

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
    "in"
};

int input_text (TEXT* data)
{
    my_assert (data != NULL);

    data->file_name_input = (const char *) "D:\\vscode\\calculator\\ass_output.txt";

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
        if (sscanf (data->buf, "%d", &data->cmd[id].command) != 1)
        {
            return ERR_COMMAND;
        }

        sprintf (buf, "%d", data->cmd[id].command);
        data->buf = move_point (data, buf);     

        if (data->cmd[id].command == PUSH)
        {
            if (sscanf (data->buf, "%d", &data->cmd[id].argc) != 1)
            {
                return ERR_ARGC;
            }

            sprintf (buf, "%d", data->cmd[id].argc);
            data->buf = move_point (data, buf);  
        }
        else if (data->cmd[id].command >= HLT && data->cmd[id].command <= IN)
        {
            data->cmd[id].argc = -1;
        }
        else
        {
            return ERR_COMMAND;
        }
    }   
    return ERR_NO;
}

static char *move_point (TEXT *data, const char *buf)
{
    my_assert (data != NULL);
    my_assert (buf != NULL);

    int len = strlen (buf);
    data->buf += len + 1;

    while (*data->buf == ' ' || *data->buf == '\n')
    {
        data->buf++;
    }   

    return data->buf;
}

static size_t number_of_lines (const char *data, const size_t size)
{
    my_assert (data != NULL);

    size_t n = 1;

    for (size_t i = 1; i < size; i++)
    {
        if (data[i] == '\n' && data[i - 1] != '\n')
        {
            n++;
        }
    }

    return n;
}

size_t get_file_size (FILE *stream)
{
    my_assert (stream != NULL);

    size_t start = ftell (stream);
    fseek (stream, start, SEEK_END);
    size_t size_file = ftell (stream);
    rewind (stream);

    return size_file;
}

void print_text (TEXT *data)
{
    my_assert (data != NULL);
    
    for (size_t id = 0; id < data->n_comms; id++)
    {
        if (data->cmd[id].command == 2)
        {   
            fprintf (data->fp_print, "%s %d\n", COMMAND[data->cmd[id].command], (data->cmd[id].argc));
        }
        else if (data->cmd[id].command == 0)
        {
            fprintf (data->fp_print, "%s", COMMAND[data->cmd[id].command]);
        }
        else
        {
            fprintf (data->fp_print, "%s\n", COMMAND[data->cmd[id].command]);
        }
    }
}
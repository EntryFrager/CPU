#include "ass.h"

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

int input_text (TEXT* data)
{
    my_assert (data != NULL);

    data->file_name_input = (const char *) "..\\include\\ass_input.txt";

    data->fp_input = fopen (data->file_name_input, "r + b");

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

    split_commands (data);

    return ERR_NO;
}

void split_commands (TEXT *data)
{   
    my_assert (data != NULL);

    data->n_comms =  number_of_commands (data->buf, data->size_file);

    data->cmd = (COMMANDS *) calloc (data->n_comms, sizeof(COMMANDS));
    my_assert (data->cmd != NULL);

    (data->cmd)[0].command = data->buf;
    (data->cmd)[0].size_str = 1;
    int j = 1;

    for (size_t id = 1; id <= data->size_file; id++)
    {
        if (data->buf[id] == '\n' && data->buf[id - 1] != '\n')
        {
            *(data->buf + id) = '\0';
            
            (data->cmd)[j].command = data->buf + (id + 1);

            j++;
        }
        else
        {
            (data->cmd)[j - 1].size_str++;
        }
    }   
}

#define DEF_CMD(num, code)                                                      \
    size_t cmd_len_##num = strlen (#num);                                       \
    if (strncmp (data->cmd[id].command, COMMAND[num], cmd_len_##num) == 0)      \
    {                                                                           \
        err++;                                                                  \
        code                                                                    \
    }

int print_text (TEXT *data)
{
    my_assert (data != NULL);

    for (size_t id = 0; id < data->n_comms; id++)
    {
        int err = 0;

        #include "commands.h"
        
        if (err != 1)
        {
            return ERR_COMMAND;
        }
    }

    return ERR_NO;
}

#undef DEF_CMD
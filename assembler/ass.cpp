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

    split_commands (data);

    return ERR_NO;
}

void split_commands (TEXT *data)
{   
    my_assert (data != NULL);

    data->n_comms =  number_of_lines (data->buf, data->size_file);

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

int print_text (TEXT *data)
{
    my_assert (data != NULL);

    for (size_t id = 0; id < data->n_comms; id++)
    {
        int err = 0;

        for (size_t n_cmd = 0; n_cmd < COMMAND_CNT; n_cmd++)
        {
            size_t cmd_len = strlen (COMMAND[n_cmd]);

            if (strncmp (data->cmd[id].command, COMMAND[n_cmd], cmd_len) == 0)
            {
                err++;

                if (n_cmd == PUSH)
                {
                    int value = 0;
                    char buf[256] = "";

                    for (size_t n_reg = 0; n_reg < REG_CNT; n_reg++)
                    {
                        size_t reg_len = strlen (REG[n_reg]);

                        if (strncmp ((data->cmd[id].command + cmd_len + 1), REG[n_reg], reg_len) == 0)
                        {
                            if ((*(data->cmd[id].command + cmd_len) != ' ') || (*(data->cmd[id].command + cmd_len + 1 + reg_len + 1) != '\0'))
                            {
                                return ERR_REG_PUSH;
                            }
                            else
                            {
                                fprintf (data->fp_print, "%d %d\n", PUSH + (1 << 5), n_reg + 1);

                                value = -1;
                            }
                        }
                    }

                    if (value == 0)
                    {
                        if (sscanf ((data->cmd[id].command + cmd_len + 1), "%d", &value) != 1)
                        {
                            return ERR_ARGC;
                        }

                        sprintf (buf, "%d", value);

                        if (*(data->cmd[id].command + cmd_len + 1 + strlen (buf) + 1) == '\0' && (*(data->cmd[id].command + 4) == ' '))
                        {
                            fprintf (data->fp_print, "%d %d\n", PUSH + (1 << 4), value);
                        }
                        else
                        {
                            return ERR_COMMAND;
                        }
                    }
                }
                else if (n_cmd == POP)
                {
                    int err_reg = 0;

                    if (*(data->cmd[id].command + cmd_len + 1) == '\0')
                    {
                        fprintf (data->fp_print, "%d\n", POP);
                    }
                    else
                    {
                        for (size_t n_reg = 0; n_reg < REG_CNT; n_reg++)
                        {
                            size_t reg_len = strlen (REG[n_reg]);

                            if (strncmp ((data->cmd[id].command + cmd_len + 1), REG[n_reg], reg_len) == 0)
                            {
                                err_reg++;

                                if ((*(data->cmd[id].command + cmd_len) != ' ') || (*(data->cmd[id].command + cmd_len + 1 + reg_len + 1) != '\0'))
                                {
                                    return ERR_REG_POP;
                                }
                                else
                                {
                                    fprintf (data->fp_print, "%d %d\n", POP + (1 << 5), n_reg + 1);
                                }
                            }
                        }

                        if (err_reg == 0)
                        {
                            return ERR_REG_POP;
                        }
                    }
                }
                else if (*(data->cmd[id].command + cmd_len + 1) == '\0')
                {
                    if ((id + 1) == data->n_comms)
                    {
                        fprintf (data->fp_print, "%d", n_cmd);
                    }
                    else
                    {
                        fprintf (data->fp_print, "%d\n", n_cmd);
                    }
                }
                else
                {
                    return ERR_COMMAND;
                }
                
            }
        }

        if (err != 1)
        {
            return ERR_COMMAND;
        }
    }

    return ERR_NO;
}
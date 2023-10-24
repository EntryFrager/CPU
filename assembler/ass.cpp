#include "ass.h"

char *COMMAND[] = {
    "hlt",
    "out",
    "push",
    "add",
    "sub",
    "mul",
    "div",
    "sin",
    "cos",
    "sqrt",
    "in",
    "pop"
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

    number_of_commands (data);

    data->cmd = (COMMANDS *) calloc (data->n_cmd, sizeof(COMMANDS));
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

#define DEF_CMD(name, num, have_arg, code)                                                              \
    if (strncasecmp (data->cmd[id].command, name, strlen (name)) == 0)                                  \
        {                                                                                               \
            if (get_param (&data->cmd[id], name) != 0)                                                  \
            {                                                                                           \
                return ERR_COMMAND;                                                                     \
            }                                                                                           \
            if (have_arg)                                                                               \
            {                                                                                           \
                if (data->cmd[id].reg != 0)                                                             \
                {                                                                                       \
                    fprintf (data->fp_print_txt, "%d %d\n", num + HAVE_REG, data->cmd[id].reg);         \
                    buf[counter++] = num + HAVE_REG;                                                    \
                    buf[counter++] = data->cmd[id].reg;                                                 \
                }                                                                                       \
                else                                                                                    \
                {                                                                                       \
                    fprintf (data->fp_print_txt, "%d %d\n", num + HAVE_ARG, data->cmd[id].argc);        \
                    buf[counter++] = num + HAVE_ARG;                                                    \
                    buf[counter++] = data->cmd[id].argc;                                                \
                }                                                                                       \
            }                                                                                           \
            else                                                                                        \
            {                                                                                           \
                buf[counter++] = num;                                                                   \
                if (num == 0)                                                                           \
                {                                                                                       \
                    fprintf (data->fp_print_txt, "%d", num);                                            \
                }                                                                                       \
                else                                                                                    \
                {                                                                                       \
                    fprintf (data->fp_print_txt, "%d\n", num);                                          \
                }                                                                                       \
            }                                                                                           \
        }                                                                                               \
    else

#define DEF_JUMP_CMD(name, num, code)                                                                                                                                           \
    if (strncasecmp (data->cmd[id].command, name, strlen (name)) == 0)                                                                                                          \
    {                                                                                                                                                                           \
        buf[counter++] = num + HAVE_ARG;                                                                                                                                        \
        if (sscanf (data->cmd[id].command + strlen (name) + 1, "%d", &value) == 1)                                                                                              \
        {                                                                                                                                                                       \
            buf[counter++] = value;                                                                                                                                             \
            fprintf (data->fp_print_txt, "%d  %d\n", num, value);                                                                                                               \
        }                                                                                                                                                                       \
        else                                                                                                                                                                    \
        {                                                                                                                                                                       \
            for (size_t label_pos = 0; label_pos < LABEL_CNT; label_pos++)                                                                                                      \
            {                                                                                                                                                                   \
                if (strncasecmp (data->label[label_pos].name_label, data->cmd[id].command + strlen (name) + 1, strlen (data->label[label_pos].name_label)) == 0)                \
                {                                                                                                                                                               \
                    buf[counter++] = data->label[label_pos].label_n_str;                                                                                                        \
                    fprintf (data->fp_print_txt, "%d %d\n", num + HAVE_ARG, data->label[label_pos].label_n_str);                                                                \
                    break;                                                                                                                                                      \
                }                                                                                                                                                               \
                else                                                                                                                                                            \
                {                                                                                                                                                               \
                    err = 0;                                                                                                                                                    \
                }                                                                                                                                                               \
            }                                                                                                                                                                   \
        }                                                                                                                                                                       \
        if (err == 0)                                                                                                                                                           \
        {                                                                                                                                                                       \
            return ERR_COMMAND;                                                                                                                                                 \
        }                                                                                                                                                                       \
    }                                                                                                                                                                           \
    else                                                                                                                                                                        \

int print_text (TEXT *data)
{
    my_assert (data != NULL);

    int *buf = NULL;
    int counter = 0;
    int label_count = 0;
    int value = 0;

    buf = (int *) calloc (data->n_words, sizeof (int));
    my_assert (buf != NULL);

    data->label = (LABELS *) calloc (LABEL_CNT, sizeof (LABELS));
    my_assert (data->label != NULL);

    for (size_t id = 0; id < data->n_cmd; id++)
    {
        if (*data->cmd[id].command == ':')
        {
            data->label[label_count].name_label = data->cmd[id].command + 1;
            data->label[label_count++].label_n_str = id;
        }
    }

    for (size_t id = 0; id < data->n_cmd; id++)
    {
        int err = 1;

        #include "..\include\commands.h"

        if (*data->cmd[id].command != ':')
        {
            err = 0;
        }

        if (err != 1)
        {
            return ERR_COMMAND;
        }
    }

    fwrite (buf, sizeof (int), data->n_words, data->fp_print_bin);

    free (buf);

    return ERR_NO;
}

#undef DEF_CMD

#undef DEF_JUMP_CMD

int get_param (COMMANDS *cmd, char *cmd_str)
{
    size_t cmd_len = strlen (cmd_str);

    if (isdigit (*(cmd->command + cmd_len + 1)))
    {
        for (size_t i = 0; i < cmd->size_str - cmd_len - 2; i++)
        {
            if (!isdigit (*(cmd->command + cmd_len + 1 + i)))
            {
                return ERR_COMMAND;
            }
        }

        cmd->argc = atoi (cmd->command + cmd_len + 1);
    }
    else if ((*(cmd->command + cmd_len + 1) == 'r' && *(cmd->command + cmd_len + 3) == 'x') || (*(cmd->command + cmd_len + 1) == 'R' && *(cmd->command + cmd_len + 3) == 'X'))
    {
        if (*(cmd->command + cmd_len + 2) >= 'a' && *(cmd->command + cmd_len + 2) <= 'd')
        {
            cmd->reg = *(cmd->command + cmd_len + 2) - 'a' + 1;
        }
        else if (*(cmd->command + cmd_len + 2) >= 'A' && *(cmd->command + cmd_len + 2) <= 'D')
        {
            cmd->reg = *(cmd->command + cmd_len + 2) - 'A' + 1;
        }
        else
        {
            return ERR_COMMAND;
        }
    }

    return ERR_NO;
}
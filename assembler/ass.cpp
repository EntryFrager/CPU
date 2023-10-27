/// @file ass.cpp

#include "ass.h"

/**
 * A function that reads text from a file into one buffer.
 * @param[in] data Structure containing all information
 * @param[out] code_error Returns the error code
*/

int input_text (TEXT* data)
{
    my_assert (data != NULL);

    data->file_name_input = (const char *) "..\\ass_input.txt";

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

/**
 * Function that splits a buffer into lines.
 * @param[in] data Structure containing all information
*/

void split_commands (TEXT *data)
{   
    my_assert (data != NULL);

    number_of_commands (data);

    data->cmd = (COMMANDS *) calloc (data->n_cmd, sizeof(COMMANDS));
    my_assert (data->cmd != NULL);

    (data->cmd)[0].command = data->buf;
    (data->cmd)[0].size_str = 2;
    int j = 1;

    for (size_t id = 2; id <= data->size_file; id++)
    {
        if (data->buf[id] == '\n' && data->buf[id - 3] != '\0')
        {
            *(data->buf + id - 1) = '\0';

            for (size_t pos_space = id; id <= data->size_file; pos_space++)
            {
                if (data->buf[pos_space] != '\r' && data->buf[pos_space + 1] != '\r')
                {
                    id = pos_space;
                    break;
                }
            }

            data->cmd[j].command = data->buf + (id + 1);

            j++;
        }
        else
        {
            data->cmd[j - 1].size_str++;
        }
    }
}

/**
 * Macro for code generation of commands
 * @param[in] name Сommand name
 * @param[in] num Command number
 * @param[in] have_arg The presence of argument
 * @param[in] code The code this command should execute
*/

#define DEF_CMD(name, num, have_arg, code)                                                                                                                                                                          \
    if (strncasecmp (data->cmd[id].command, name, strlen (name)) == 0)                                                                                                                                              \
        {                                                                                                                                                                                                           \
            if ((err = get_param (&data->cmd[id], name)) != 0)                                                                                                                                                      \
            {                                                                                                                                                                                                       \
                return err;                                                                                                                                                                                         \
            }                                                                                                                                                                                                       \
            if (have_arg)                                                                                                                                                                                           \
            {                                                                                                                                                                                                       \
                if (data->cmd[id].ram != VALUE_DEFAULT && data->cmd[id].reg != VALUE_DEFAULT)                                                                                                                       \
                {                                                                                                                                                                                                   \
                    fprintf (data->fp_print_txt, "|%12s|%12d|%12x|%12d|%12d|%12d|\n", name, num + HAVE_REG + HAVE_RAM, num + HAVE_REG + HAVE_RAM, data->cmd[id].argc, data->cmd[id].reg, data->cmd[id].ram);        \
                    buf[counter++] = num + HAVE_RAM + HAVE_REG;                                                                                                                                                     \
                    buf[counter++] = data->cmd[id].reg;                                                                                                                                                             \
                }                                                                                                                                                                                                   \
                else if (data->cmd[id].ram != VALUE_DEFAULT)                                                                                                                                                        \
                {                                                                                                                                                                                                   \
                    fprintf (data->fp_print_txt, "|%12s|%12d|%12x|%12d|%12d|%12d|\n", name, num + HAVE_ARG + HAVE_RAM, num + HAVE_ARG + HAVE_RAM, data->cmd[id].argc, data->cmd[id].reg, data->cmd[id].ram);        \
                    buf[counter++] = num + HAVE_RAM + HAVE_ARG;                                                                                                                                                     \
                    buf[counter++] = data->cmd[id].argc;                                                                                                                                                            \
                }                                                                                                                                                                                                   \
                else if (data->cmd[id].reg != VALUE_DEFAULT)                                                                                                                                                        \
                {                                                                                                                                                                                                   \
                    fprintf (data->fp_print_txt, "|%12s|%12d|%12x|%12d|%12d|%12d|\n", name, num + HAVE_REG, num + HAVE_REG, data->cmd[id].argc, data->cmd[id].reg, data->cmd[id].ram);                              \
                    buf[counter++] = num + HAVE_REG;                                                                                                                                                                \
                    buf[counter++] = data->cmd[id].reg;                                                                                                                                                             \
                }                                                                                                                                                                                                   \
                else                                                                                                                                                                                                \
                {                                                                                                                                                                                                   \
                    fprintf (data->fp_print_txt, "|%12s|%12d|%12x|%12d|%12d|%12d|\n", name, num + HAVE_ARG, num + HAVE_ARG, data->cmd[id].argc, data->cmd[id].reg, data->cmd[id].ram);                              \
                    buf[counter++] = num + HAVE_ARG;                                                                                                                                                                \
                    buf[counter++] = data->cmd[id].argc;                                                                                                                                                            \
                }                                                                                                                                                                                                   \
            }                                                                                                                                                                                                       \
            else                                                                                                                                                                                                    \
            {                                                                                                                                                                                                       \
                buf[counter++] = num;                                                                                                                                                                               \
                fprintf (data->fp_print_txt, "|%12s|%12d|%12x|%12d|%12d|%12d|\n", name, num, num, data->cmd[id].argc, data->cmd[id].reg, data->cmd[id].ram);                                                        \
            }                                                                                                                                                                                                       \
        }                                                                                                                                                                                                           \
    else

/**
 * Macro for code generation of commands like jump
 * @param[in] name Сommand name
 * @param[in] num Command number
 * @param[in] code The code this command should execute
*/

#define DEF_JUMP_CMD(name, num, code)                                                                                                                                                                               \
    if (strncasecmp (data->cmd[id].command, name, strlen (name)) == 0)                                                                                                                                              \
    {                                                                                                                                                                                                               \
        buf[counter++] = num + HAVE_ARG;                                                                                                                                                                            \
        if (sscanf (data->cmd[id].command + strlen (name) + 1, "%d", &value) == 1)                                                                                                                                  \
        {                                                                                                                                                                                                           \
            buf[counter++] = value;                                                                                                                                                                                 \
            fprintf (data->fp_print_txt, "|%12s|%12d|%12x|%12d|%12d|%12d|\n", name, num + HAVE_ARG, num + HAVE_ARG, value, data->cmd[id].reg, data->cmd[id].ram);                                                   \
        }                                                                                                                                                                                                           \
        else                                                                                                                                                                                                        \
        {                                                                                                                                                                                                           \
            for (size_t label_pos = 0; label_pos < LABEL_CNT; label_pos++)                                                                                                                                          \
            {                                                                                                                                                                                                       \
                if (strncasecmp (data->label[label_pos].name_label, data->cmd[id].command + strlen (name) + 1, strlen (data->label[label_pos].name_label)) == 0)                                                    \
                {                                                                                                                                                                                                   \
                    buf[counter++] = data->label[label_pos].label_n_str;                                                                                                                                            \
                        fprintf (data->fp_print_txt, "|%12s|%12d|%12x|%12d|%12d|%12d|\n", name, num + HAVE_ARG, num + HAVE_ARG, data->label[label_pos].label_n_str, data->cmd[id].reg, data->cmd[id].ram);          \
                    err = ERR_NO;                                                                                                                                                                                   \
                    break;                                                                                                                                                                                          \
                }                                                                                                                                                                                                   \
                else                                                                                                                                                                                                \
                {                                                                                                                                                                                                   \
                    err = ERR_LABEL;                                                                                                                                                                                \
                }                                                                                                                                                                                                   \
            }                                                                                                                                                                                                       \
        }                                                                                                                                                                                                           \
        if (err == ERR_LABEL)                                                                                                                                                                                       \
        {                                                                                                                                                                                                           \
            return ERR_LABEL;                                                                                                                                                                                       \
        }                                                                                                                                                                                                           \
    }                                                                                                                                                                                                               \
    else                                                                                                                                                                                                            \

/**
 * A function that outputs machine code to a new file.
 * @param[in] data Structure containing all information
 * @param[out] code_error Returns the error code
*/

int print_text (TEXT *data)
{
    my_assert (data != NULL);

    int counter = 0;
    int label_count = 0;
    int value = 0;

    data->label = (LABELS *) calloc (LABEL_CNT, sizeof (LABELS));
    my_assert (data->label != NULL);

    for (size_t id = 0; id < data->n_cmd; id++)
    {
        if (*data->cmd[id].command == ':')
        {
            if (label_count >= LABEL_CNT)
            {
                return ERR_LABEL;
            }
            data->label[label_count].name_label = data->cmd[id].command + 1;
            data->label[label_count].label_n_str = id + 1 - (++label_count);
        }
    }

    data->n_words -= label_count;

    int *buf = NULL;
    buf = (int *) calloc (data->n_words, sizeof (int));
    my_assert (buf != NULL);

    fprintf (data->fp_print_txt, "+------------+------------+------------+------------+------------+------------+\n");
    fprintf (data->fp_print_txt, "|  NAME_CMD  |  CODE_CMD  |  HEX_SPEAK |  VALUE_ARG |  VALUE_REG |  VALUE_RAM |\n");
    fprintf (data->fp_print_txt, "+------------+------------+------------+------------+------------+------------+\n");

    for (size_t id = 0; id < data->n_cmd; id++)
    {
        int err = ERR_NO;

        #include "..\include\commands.h"
        #include "..\include\jump_cmd.h"

        if (*data->cmd[id].command != ':')
        {
            err = ERR_COMMAND;
        }

        if (err != ERR_NO)
        {
            return err;
        }
        if (id == (data->n_cmd - 1) && *data->cmd[id].command != ':')
        {
            fprintf (data->fp_print_txt, "+------------+------------+------------+------------+------------+------------+");
        }
        else if (*data->cmd[id].command != ':')
        {
            fprintf (data->fp_print_txt, "+------------+------------+------------+------------+------------+------------+\n");
        }
    }

    fwrite (buf, sizeof (int), data->n_words, data->fp_print_bin);

    free (buf);
    buf = NULL;

    return ERR_NO;
}

#undef DEF_CMD

#undef DEF_JUMP_CMD

/**
 * Argument checking macro
 * @param[in] len Takes the length value after the command, before the argument
*/

#define DEF_GET_PARAM(len)                                                                                                                                                                                  \
    if (isdigit (*(cmd->command + cmd_len + len)))                                                                                                                                                          \
    {                                                                                                                                                                                                       \
        for (size_t i = 0; i < cmd->size_str - cmd_len - 2 - len; i++)                                                                                                                                      \
        {                                                                                                                                                                                                   \
            if (!isdigit (*(cmd->command + cmd_len + len + i)))                                                                                                                                             \
            {                                                                                                                                                                                               \
                return ERR_ARGC;                                                                                                                                                                            \
            }                                                                                                                                                                                               \
        }                                                                                                                                                                                                   \
        cmd->argc = atoi (cmd->command + cmd_len + len);                                                                                                                                                    \
    }                                                                                                                                                                                                       \
    else if ((*(cmd->command + cmd_len + len) == 'r' && *(cmd->command + cmd_len + 2 + len) == 'x') || (*(cmd->command + cmd_len + len) == 'R' && *(cmd->command + cmd_len + 2 + len) == 'X'))              \
    {                                                                                                                                                                                                       \
        if (*(cmd->command + cmd_len + 1 + len) >= 'a' && *(cmd->command + cmd_len + 1 + len) <= 'd')                                                                                                       \
        {                                                                                                                                                                                                   \
            cmd->reg = *(cmd->command + cmd_len + 1 + len) - 'a' + 1;                                                                                                                                       \
        }                                                                                                                                                                                                   \
        else if (*(cmd->command + cmd_len + 1 + len) >= 'A' && *(cmd->command + cmd_len + 1 + len) <= 'D')                                                                                                  \
        {                                                                                                                                                                                                   \
            cmd->reg = *(cmd->command + cmd_len + 1 + len) - 'A' + 1;                                                                                                                                       \
        }                                                                                                                                                                                                   \
        else                                                                                                                                                                                                \
        {                                                                                                                                                                                                   \
            return ERR_REG;                                                                                                                                                                                 \
        }                                                                                                                                                                                                   \
    }                                                                                                                                                                                                       \
    else

/**
 * A function that detects whether a command has an argument or not.
 * @param[in] cmd A structure containing all information about the command line
 * @param[in] cmd_str A string containing the command name
 * @param[out] code_error Returns the error code
*/

int get_param (COMMANDS *cmd, char *cmd_str)
{
    size_t cmd_len = strlen (cmd_str);

    DEF_GET_PARAM (1)
    if (*(cmd->command + cmd_len + 1) == '[' && *(cmd->command + cmd->size_str - 2) == ']')
    {   
        DEF_GET_PARAM (2)
        
        {
            return ERR_REG;
        }

        cmd->ram = 1;
    }
    else if (cmd_len == (cmd->size_str + 1))
    {
        return ERR_COMMAND;
    }

    return ERR_NO;
}

#undef DEF_GET_PARAM
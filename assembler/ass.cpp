/// @file ass.cpp

#include "ass.h"

#define CHECK_ERROR(expr) int code_error = 0; if ((code_error = expr) != ERR_NO) return code_error;

/**
 * A function that reads text from a file into one buffer.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int input_text (SPU *spu)
{
    my_assert (spu != NULL);

    spu->fp_input = fopen (spu->file_name_input, "r + b");

    if (spu->fp_input == NULL)
    {
        return ERR_FOPEN;
    }

    spu->size_file = get_file_size (spu->fp_input);

    spu->buf_input = (char *) calloc (spu->size_file + 1, sizeof (char));
    my_assert (spu->buf_input != NULL);

    size_t read_size = fread (spu->buf_input, sizeof (char), spu->size_file, spu->fp_input);

    if (read_size != spu->size_file)
    {
        return ERR_FREAD;
    }

    *(spu->buf_input + spu->size_file) = '\0';

    if (fclose (spu->fp_input) != 0)
    {
        return ERR_FCLOSE;
    }

    split_commands (spu);

    clean_comment (spu);

    return ERR_NO;
}

/**
 * Function that splits a buffer into lines.
 * @param[in] spu Structure containing all information
*/

void split_commands (SPU *spu)
{
    my_assert (spu != NULL);

    number_of_commands (spu);

    spu->cmd = (COMMANDS *) calloc (spu->n_cmd, sizeof (COMMANDS));
    my_assert (spu->cmd != NULL);

    size_t buf_pos_count = 0;

    for (size_t ip = 0; ip < spu->n_cmd; ip++)
    {
        while (isspace (*(spu->buf_input + buf_pos_count)) != 0 && buf_pos_count < spu->size_file)
        {
            buf_pos_count++;
        }

        spu->cmd[ip].command = spu->buf_input + buf_pos_count;

        while (*(spu->buf_input + buf_pos_count) != '\r' && buf_pos_count < spu->size_file)
        {
            buf_pos_count++;
            spu->cmd[ip].size_str++;
        }

        *(spu->buf_input + buf_pos_count) = '\0';
        buf_pos_count += 2;
    }
}

/**
 * Function that removes comments.
 * @param[in] spu Structure containing all information
*/

void clean_comment (SPU *spu)
{
    my_assert (spu != NULL);
    
    for (size_t ip = 0; ip < spu->n_cmd; ip++)
    {
        int pos = 0;
        int space_count = 0;

        while (*(spu->cmd[ip].command + pos) != '\0')
        {
            if (*(spu->cmd[ip].command + pos) == ';')
            {
                *(spu->cmd[ip].command + pos) = '\0';
                spu->cmd[ip].size_str = pos;
                break;
            }
            else
            {
                pos++;
            }
            
            space_count = 0;

            while (isspace (*(spu->cmd[ip].command + pos)) != 0)
            {
                space_count++;
                pos++;
            }
        }

        spu->cmd[ip].size_str -= space_count;

        *(spu->cmd[ip].command + spu->cmd[ip].size_str) = '\0';
    }
}

#define DEF_CMD(name, num, have_arg, ...)                                               \
    if (strncasecmp (spu->cmd[ip].command, name, sizeof (name) - 1) == 0)               \
    {                                                                                   \
        spu->cmd[ip].cmd_code = num;                                                    \
        if (have_arg)                                                                   \
        {                                                                               \
            CHECK_ERROR (param_check (&spu->cmd[ip], spu->label, sizeof (name) - 1))    \
        }                                                                               \
        *(spu->cmd[ip].command + sizeof (name) - 1) = '\0';                             \
    }                                                                                   \
    else

/**
 * A function that compiles user code into machine code and outputs it to a file.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int compare_command (SPU *spu)
{    
    my_assert (spu != NULL);

    spu->label = (LABELS *) calloc (LABEL_CNT, sizeof (LABELS));
    my_assert (spu->label != NULL);

    size_t label_count = 0;

    for (size_t ip = 0; ip < spu->n_cmd; ip++)
    {
        if (label_count < LABEL_CNT)
        {
            if (*(spu->cmd[ip].command + spu->cmd[ip].size_str - 1) == ':')
            {
                spu->label[label_count].name_label  = spu->cmd[ip].command;
                spu->label[label_count].label_n_str = ip - label_count;
                spu->label[label_count].size_label  = spu->cmd[ip].size_str - 1;
                label_count++;
            }
        }
        else
        {
            return ERR_LABEL;
        }
    }

    spu->n_words -= label_count;

    spu->fp_print_txt = fopen (spu->file_name_print_txt, "w");

    if (spu->fp_print_txt == NULL)
    {
        return ERR_FOPEN;
    }

    fprintf (spu->fp_print_txt, "+------------+------------+------------+------------+------------+\n");
    fprintf (spu->fp_print_txt, "|  NAME_CMD  |  HEX_SPEAK |  VALUE_ARG |  VALUE_REG |  VALUE_RAM |\n");
    fprintf (spu->fp_print_txt, "+------------+------------+------------+------------+------------+\n");

    int *buf = (int *) calloc (sizeof (int), spu->n_words);
    my_assert (buf != NULL);

    int counter = 0;

    for (size_t ip = 0; ip < spu->n_cmd; ip++)
    {
        if (*(spu->cmd[ip].command + spu->cmd[ip].size_str - 1) != ':')
        {
            #include "..\include\commands.h"
            #include "..\include\jump_cmd.h"

            {
                return ERR_COMMAND;
            }
        }

        if (*(spu->cmd[ip].command + spu->cmd[ip].size_str - 1) != ':')
        {
            fprintf (spu->fp_print_txt, "|%12s|%12x|%12d|%12d|%12d|\n", spu->cmd[ip].command, spu->cmd[ip].cmd_code, spu->cmd[ip].argc, spu->cmd[ip].reg, spu->cmd[ip].ram);

            fprintf (spu->fp_print_txt, "+------------+------------+------------+------------+------------+\n");

            buf[counter++] = spu->cmd[ip].cmd_code;

            if (spu->cmd[ip].cmd_code & HAVE_REG)
            {
                buf[counter++] = spu->cmd[ip].reg;
            }
            else if (spu->cmd[ip].cmd_code & HAVE_ARG)
            {
                buf[counter++] = spu->cmd[ip].argc;
            }
        }
    }

    if (fclose (spu->fp_print_txt) != 0)
    {
        return ERR_FCLOSE;
    }

    spu->fp_print_bin = fopen (spu->file_name_print_bin, "w + b");

    if (spu->fp_print_bin == NULL)
    {
        return ERR_FOPEN;
    }

    fwrite (buf, sizeof (int), spu->n_words, spu->fp_print_bin);

    if (fclose (spu->fp_print_bin) != 0)
    {
        return ERR_FCLOSE;
    }

    free (buf);
    buf = NULL;

    return ERR_NO;
}

#undef DEF_CMD

/**
 * A function that checks whether a command has a parameter.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int param_check (COMMANDS *cmd, LABELS *label, int cmd_len)
{
    my_assert (cmd != NULL);
    my_assert (label != NULL);

    if (*(cmd->command + cmd_len + 1) == '[' && *(cmd->command + cmd->size_str - 1) == ']')
    {
        CHECK_ERROR (get_param (cmd, label, cmd_len, 2))

        cmd->ram = 1;

        cmd->cmd_code |= HAVE_RAM;
    }
    else
    {
        CHECK_ERROR (get_param (cmd, label, cmd_len, 1))
    }

    return ERR_NO;
}

/**
 * Function that finds a parameter for a command.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int get_param (COMMANDS *cmd, LABELS *label, int cmd_len, int len)
{
    my_assert (cmd != NULL);
    my_assert (label != NULL);

    /*if (isdigit (*(cmd->command + cmd_len + len)) || *(cmd->command + cmd_len + len) == '-')
    {
        for (size_t ip = 1; ip < cmd->size_str - cmd_len - len - 1; ip++)
        {
            if (isdigit (*(cmd->command + cmd_len + len + ip)) == 0)
            {
                return ERR_ARGC;
            }
        }

        cmd->argc = atoi (cmd->command + cmd_len + len);

        if (*(cmd->command + cmd_len + len) == '-')
        {
            cmd->argc = cmd->argc * (-1);
        }

        printf ("%d\n", cmd->argc);

        cmd->cmd_code |= HAVE_ARG;
    }*/
    if (sscanf (cmd->command + cmd_len + len, "%d", &cmd->argc) == 1)
    {
        cmd->cmd_code |= HAVE_ARG;
    }
    else if (toupper (*(cmd->command + cmd_len + len)) == 'R' && toupper (*(cmd->command + cmd_len + len + 2)) == 'X')
    {
        if (toupper (*(cmd->command + cmd_len + len + 1)) >= 'A' || toupper (*(cmd->command + cmd_len + len + 1)) <= 'D')
        {
            cmd->reg = toupper(*(cmd->command + cmd_len + len + 1)) - 'A' + 1;
            cmd->cmd_code |= HAVE_REG;
        }
        else
        {
            return ERR_REG;
        }
    }
    else
    {
        for (size_t ip = 0; ip < LABEL_CNT; ip++)
        {
            if (label[ip].name_label != NULL)
            {
                if (strncasecmp (cmd->command + cmd_len + len, label[ip].name_label, label[ip].size_label) == 0)
                {
                    cmd->argc = label[ip].label_n_str;
                    cmd->cmd_code |= HAVE_ARG;
                    
                    return ERR_NO;
                }
            }
            else
            {
                return ERR_LABEL;
            }
        }
    }

    return ERR_NO;
}
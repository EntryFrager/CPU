/// @file ass.cpp

#include "ass.h"

#define CHECK_ERROR(expr) int code_error = 0; if ((code_error = expr) != ERR_NO) return code_error;

/**
 * A function that reads text from a file into one buffer.
 * @param[in] proc Structure containing all information
 * @param[out] code_error Returns the error code
*/

int input_text (SPU *proc)
{
    my_assert (proc != NULL);

    proc->fp_input = fopen (proc->file_name_input, "r + b");

    if (proc->fp_input == NULL)
    {
        return ERR_FOPEN;
    }

    proc->size_file = get_file_size (proc->fp_input);

    proc->buf_input = (char *) calloc (proc->size_file + 1, sizeof (char));
    my_assert (proc->buf_input != NULL);

    size_t read_size = fread (proc->buf_input, sizeof (char), proc->size_file, proc->fp_input);

    if (read_size != proc->size_file)
    {
        return ERR_FREAD;
    }

    *(proc->buf_input + proc->size_file) = '\0';

    if (fclose (proc->fp_input) != 0)
    {
        return ERR_FCLOSE;
    }

    split_commands (proc);

    return ERR_NO;
}

void split_commands (SPU *proc)
{
    my_assert (proc != NULL);

    number_of_commands (proc);

    proc->cmd = (COMMANDS *) calloc (proc->n_cmd, sizeof (COMMANDS));

    size_t buf_pos_count = 0;

    for (size_t ip = 0; ip < proc->n_cmd; ip++)
    {
        while (isspace (*(proc->buf_input + buf_pos_count)) != 0 && buf_pos_count < proc->size_file)
        {
            buf_pos_count++;
        }

        proc->cmd[ip].command = proc->buf_input + buf_pos_count;

        while (*(proc->buf_input + buf_pos_count) != '\r' && buf_pos_count < proc->size_file)
        {
            buf_pos_count++;
            proc->cmd[ip].size_str++;
        }

        *(proc->buf_input + buf_pos_count) = '\0';
        buf_pos_count += 2;
    }
}

#define DEF_CMD(name, num, have_arg, ...)                                               \
    if (strncasecmp (proc->cmd[ip].command, name, sizeof (name) - 1) == 0)              \
    {                                                                                   \
        proc->cmd[ip].cmd_code = num;                                                   \
        if (have_arg)                                                                   \
        {                                                                               \
            CHECK_ERROR (param_check (&proc->cmd[ip], &proc->label, sizeof (name) - 1)) \
        }                                                                               \
        *(proc->cmd[ip].command + sizeof (name) - 1) = '\0';                            \
    }                                                                                   \
    else

int compare_command (SPU *proc)
{    
    my_assert (proc != NULL);

    proc->label = (LABELS *) calloc (LABEL_CNT, sizeof (LABELS));
    my_assert (proc->label != NULL);

    size_t label_count = 0;

    for (size_t ip = 0; ip < proc->n_cmd; ip++)
    {
        if (label_count < LABEL_CNT)
        {
            if (*(proc->cmd[ip].command + proc->cmd[ip].size_str - 1) == ':')
            {
                proc->label[label_count].name_label  = proc->cmd[ip].command;
                proc->label[label_count].label_n_str = ip - label_count;
                label_count++;
            }
        }
    }

    proc->n_words -= label_count;

    proc->fp_print_txt = fopen (proc->file_name_print_txt, "w");

    if (proc->fp_print_txt == NULL)
    {
        return ERR_FOPEN;
    }

    fprintf (proc->fp_print_txt, "+------------+------------+------------+------------+------------+\n");
    fprintf (proc->fp_print_txt, "|  NAME_CMD  |  HEX_SPEAK |  VALUE_ARG |  VALUE_REG |  VALUE_RAM |\n");
    fprintf (proc->fp_print_txt, "+------------+------------+------------+------------+------------+\n");

    int *buf = (int *) calloc (sizeof (int), proc->n_words);
    my_assert (buf != NULL);

    int counter = 0;

    for (size_t ip = 0; ip < proc->n_cmd; ip++)
    {
        #include "..\include\commands.h"
        #include "..\include\jump_cmd.h"

        if (*(proc->cmd[ip].command + proc->cmd[ip].size_str - 1) != ':')
        {
            return ERR_COMMAND;
        }

        if (*(proc->cmd[ip].command + proc->cmd[ip].size_str - 1) != ':')
        {
            fprintf (proc->fp_print_txt, "|%12s|%12x|%12d|%12d|%12d|\n", proc->cmd[ip].command, proc->cmd[ip].cmd_code, proc->cmd[ip].argc, proc->cmd[ip].reg, proc->cmd[ip].ram);

            fprintf (proc->fp_print_txt, "+------------+------------+------------+------------+------------+\n");

            buf[counter++] = proc->cmd[ip].cmd_code;

            if (proc->cmd[ip].cmd_code & HAVE_REG)
            {
                buf[counter++] = proc->cmd[ip].reg;
            }
            else if (proc->cmd[ip].cmd_code & HAVE_ARG)
            {
                buf[counter++] = proc->cmd[ip].argc;
            }
        }
    }

    if (fclose (proc->fp_print_txt) != 0)
    {
        return ERR_FCLOSE;
    }

    proc->fp_print_bin = fopen (proc->file_name_print_bin, "w + b");

    if (proc->fp_print_bin == NULL)
    {
        return ERR_FOPEN;
    }

    fwrite (buf, sizeof (int), proc->n_words, proc->fp_print_bin);

    if (fclose (proc->fp_print_bin) != 0)
    {
        return ERR_FCLOSE;
    }

    free(buf);
    buf = NULL;

    return ERR_NO;
}

#undef DEF_CMD

int param_check (COMMANDS *cmd, LABELS **label, int cmd_len)
{
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

int get_param (COMMANDS *cmd, LABELS **label, int cmd_len, int len)
{
    if (isdigit (*(cmd->command + cmd_len + len)))
    {
        for (size_t ip = 0; ip < cmd->size_str - cmd_len - len - 1; ip++)
        {
            if (isdigit (*(cmd->command + cmd_len + len + ip)) == 0)
            {
                return ERR_ARGC;
            }
        }

        cmd->argc = atoi (cmd->command + cmd_len + len);

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
            if (label[ip] != NULL)
            {
                if (strncasecmp (cmd->command + cmd_len + len, label[ip]->name_label, sizeof (label[ip]->name_label) - 2) == 0)
                {
                    cmd->argc = label[ip]->label_n_str;
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
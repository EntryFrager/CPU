/// @file ass.cpp

#include "ass.h"

static int check_argc (COMMANDS *cmd, int cmd_len, int len);

static int check_reg  (COMMANDS *cmd, int cmd_len, int len);

static int process_label  (SPU *spu, size_t ip, int cmd_len, int len);

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
            code_error = param_check (spu, ip, sizeof (name) - 1);                      \
            CHECK_ERROR_RETURN (code_error)                                             \
            counter_ip++;                                                               \
        }                                                                               \
    }                                                                                   \
    else

/**
 * A function that compiles user code into machine code and outputs it to a file.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int pars_command (SPU *spu, size_t n_compile)
{
    my_assert (spu != NULL);

    int code_error = 0;

    if (n_compile == 1)
    {
        spu->label = (LABELS *) calloc (LABEL_CNT, sizeof (LABELS));
        my_assert (spu->label != NULL);
    }
    else if (n_compile == 2)
    {
        printf ("%d\n", spu->n_words);
        spu->buf_output = (int *) calloc (sizeof (int), spu->n_words);
        my_assert (spu->buf_output != NULL);

        fprintf (spu->fp_print_txt, "+------------+------------+------------+------------+\n");
        fprintf (spu->fp_print_txt, "|  NAME_CMD  |  HEX_SPEAK |  VALUE_ARG |  VALUE_REG |\n");
        fprintf (spu->fp_print_txt, "+------------+------------+------------+------------+\n");
    }    

    size_t counter_buf = 0;
    size_t counter_ip  = 0;

    for (size_t ip = 0; ip < spu->n_cmd; ip++)
    {
        if (spu->n_label <= LABEL_CNT)
        {
            if (*(spu->cmd[ip].command + spu->cmd[ip].size_str - 1) == ':' && n_compile == 1)
            {
                spu->label[spu->n_label].name_label  = spu->cmd[ip].command;
                spu->label[spu->n_label].label_ip = counter_ip - spu->n_label;
                spu->label[spu->n_label].size_label  = spu->cmd[ip].size_str - 1;
                spu->n_label++;
            }
            else if (*(spu->cmd[ip].command + spu->cmd[ip].size_str - 1) != ':')
            {
                #include "..\include\commands.h"
                #include "..\include\jump_cmd.h"

                {
                    return ERR_COMMAND;
                }

                if (n_compile == 2)
                {
                    fprintf (spu->fp_print_txt, "|%12s|%12x|%12d|%12d|\n", spu->cmd[ip].command, spu->cmd[ip].cmd_code, spu->cmd[ip].argc, spu->cmd[ip].reg);
                    fprintf (spu->fp_print_txt, "+------------+------------+------------+------------+\n");

                    counter_buf = write_buf (&spu->cmd[ip], spu->buf_output, counter_buf);
                }
            }
        }
        else
        {
            return ERR_LABEL;
        }

        counter_ip++;
    }

    if (n_compile == 1)
    {
        spu->n_words -= spu->n_label;
    }
    
    return ERR_NO;
}

#undef DEF_CMD

/**
 * A function that writes the converted command to the buffer.
 * @param[in] cmd A structure that stores all information about a command
 * @param[in] buf Write buffer
 * @param[in] counter Counter where to write to buffer
 * @param[out] counter Counter shifted to next element in buffer
*/

int write_buf (COMMANDS *cmd, int *buf, size_t counter)
{
    my_assert (cmd != NULL);
    my_assert (buf != NULL);

    buf[counter++] = cmd->cmd_code;

    if (cmd->cmd_code & HAVE_REG)
    {
        buf[counter++] = cmd->reg;
    }
    else if (cmd->cmd_code & HAVE_ARG)
    {
        buf[counter++] = cmd->argc;
    }

    return counter;
}

/**
 * A function that checks whether a command has a parameter.
 * @param[in] spu Structure containing all information
 * @param[in] label An array of structures storing all information about labels
 * @param[in] cmd_len Command name length
 * @param[out] code_error Returns the error code
*/

int param_check (SPU *spu, size_t ip, int cmd_len)
{
    my_assert (spu != NULL);

    int code_error = 0;

    if (*(spu->cmd[ip].command + cmd_len + 1) == '[' && *(spu->cmd[ip].command + spu->cmd[ip].size_str - 1) == ']')
    {
        code_error = get_param (spu, ip, cmd_len, 2);
        CHECK_ERROR_RETURN (code_error)

        spu->cmd[ip].cmd_code |= HAVE_RAM;
    }
    else
    {
        code_error = get_param (spu, ip, cmd_len, 1);
        CHECK_ERROR_RETURN (code_error)
    }

    return ERR_NO;
}

/**
 * Function that finds a parameter for a command.
 * @param[in] spu Structure containing all information
 * @param[in] label An array of structures storing all information about labels
 * @param[in] cmd_len Command name length
 * @param[in] len 1 if space only, 2 if RAM is used
 * @param[out] code_error Returns the error code
*/

int get_param (SPU *spu, size_t ip, int cmd_len, int len)
{
    my_assert (spu != NULL);

    int code_error = 0;

    if ((code_error = check_argc (&spu->cmd[ip], cmd_len, len)) == HAVE_NOT_PARAM)
    {
        if ((code_error = check_reg  (&spu->cmd[ip], cmd_len, len)) == HAVE_NOT_PARAM)
        {
            return process_label (spu, ip, cmd_len, len);
        }
        else
        {
            return code_error;
        }
    }
    else
    {
        return code_error;
    }

    return ERR_NO;
}

/**
 *  Function that checks the argument of a command.
 * @param[in] cmd A structure that stores all information about a command
 * @param[in] cmd_len Command name length
 * @param[in] len 1 if space only, 2 if RAM is used
 * @param[out] code_error Returns the error code
*/

static int check_argc (COMMANDS *cmd, int cmd_len, int len)
{
    my_assert (cmd != NULL);

    if (isdigit (*(cmd->command + cmd_len + len)) || *(cmd->command + cmd_len + len) == '-')
    {
        for (size_t ip = 1; ip < cmd->size_str - cmd_len - len - 1; ip++)
        {
            if (isdigit (*(cmd->command + cmd_len + len + ip)) == 0)
            {
                return ERR_ARGC;
            }
        }        

        if (*(cmd->command + cmd_len + len) == '-')
        {
            cmd->argc = atoi (cmd->command + cmd_len + len + 1);
            cmd->argc = cmd->argc * (-1);
        }
        else
        {
            cmd->argc = atoi (cmd->command + cmd_len + len);   
        }

        cmd->cmd_code |= HAVE_ARG;
        *(cmd->command + cmd_len) = '\0';

        return ERR_NO;
    }
    
    return HAVE_NOT_PARAM;
}

/**
 * Function that checks the case of a command.
 * @param[in] cmd A structure that stores all information about a command
 * @param[in] cmd_len Command name length
 * @param[in] len 1 if space only, 2 if RAM is used
 * @param[out] code_error Returns the error code
*/

static int check_reg (COMMANDS *cmd, int cmd_len, int len)
{
    my_assert (cmd != NULL);

    if (toupper (*(cmd->command + cmd_len + len)) == 'R' && toupper (*(cmd->command + cmd_len + len + 2)) == 'X')
    {
        if (toupper (*(cmd->command + cmd_len + len + 1)) >= 'A' || toupper (*(cmd->command + cmd_len + len + 1)) <= 'D')
        {
            cmd->reg = toupper(*(cmd->command + cmd_len + len + 1)) - 'A' + 1;
            cmd->cmd_code |= HAVE_REG;
            *(cmd->command + cmd_len) = '\0';

            return ERR_NO;
        }
        else
        {
            return ERR_REG;
        }
    }

    return HAVE_NOT_PARAM;
}

/**
 * A function that checks the label of a command.
 * @param[in] cmd A structure that stores all information about a command
 * @param[in] label An array of structures storing all information about labels
 * @param[in] cmd_len Command name length
 * @param[in] len 1 if space only, 2 if RAM is used
 * @param[out] code_error Returns the error code
*/

static int process_label (SPU *spu, size_t ip, int cmd_len, int len)
{
    my_assert (spu != NULL);

    for (size_t pos_label = 0; pos_label <= spu->n_label; pos_label++)
    {
        if (spu->label[pos_label].name_label != NULL)
        {
            if (strncasecmp (spu->cmd[ip].command + cmd_len + len, spu->label[pos_label].name_label, spu->label[pos_label].size_label) == 0)
            {
                spu->cmd[ip].argc = spu->label[pos_label].label_ip;
                spu->cmd[ip].cmd_code |= HAVE_ARG;
                *(spu->cmd[ip].command + cmd_len) = '\0';
                
                return ERR_NO;
            }
        }
        else
        {
            spu->cmd[ip].argc = 0;

            return ERR_NO;
        }
    }

    return ERR_LABEL;
}
/// @file func.cpp

#include "func.h"

/**
 * Function that reads information from a file.
 * @param[in] data Structure containing all information
 * @param[out] code_error Code error
*/

int input_text (TEXT* data)
{
    my_assert (data != NULL);

    data->file_name_input = (const char *) "..\\ass_output.bin";

    data->fp_input = fopen (data->file_name_input, "r + b");

    if (data->fp_input == NULL)
    {
        return ERR_FOPEN;
    }

    data->size_file = get_file_size (data->fp_input) / 4;

    data->buf = (int *) calloc (data->size_file * sizeof (int), sizeof (char));
    my_assert (data->buf != NULL);

    size_t read_size = fread (data->buf, sizeof (int), data->size_file, data->fp_input);

    if (read_size != data->size_file)
    {
        return ERR_FREAD;
    }

    if (fclose (data->fp_input) != 0)
    {
        return ERR_FCLOSE;
    }

    int code_error = split_commands (data);

    return code_error;
}

/**
 * Macro for code generation of commands
 * @param[in] name Сommand name
 * @param[in] num Command number
 * @param[in] have_arg The presence of argument
 * @param[in] code The code this command should execute
*/

#define DEF_CMD(name, num, have_param, code)                                            \
    case (num):                                                                         \
        {                                                                               \
            data->cmd[pos_cmd].command = num;                                           \
            if (have_param)                                                             \
            {                                                                           \
                if (command == (num + HAVE_RAM + HAVE_REG))                             \
                {                                                                       \
                    data->cmd[pos_cmd].ram = 1;                                         \
                    data->cmd[pos_cmd].reg = data->buf[++id];                           \
                }                                                                       \
                else if (command == (num + HAVE_RAM + HAVE_ARG))                        \
                {                                                                       \
                    data->cmd[pos_cmd].ram = 1;                                         \
                    data->cmd[pos_cmd].argc = data->buf[++id];                          \
                }                                                                       \
                else if (command == (num + HAVE_ARG))                                   \
                {                                                                       \
                    data->cmd[pos_cmd].argc = data->buf[++id];                          \
                }                                                                       \
                else if (command == (num + HAVE_REG))                                   \
                {                                                                       \
                    data->cmd[pos_cmd].reg  = data->buf[++id];                          \
                }                                                                       \
                else                                                                    \
                {                                                                       \
                    printf ("%s\n", name);\
                    return ERR_COMMAND;                                                 \
                }                                                                       \
            }                                                                           \
            break;                                                                      \
        }
    
/**
 * Macro for code generation of commands like jump
 * @param[in] name Сommand name
 * @param[in] num Command number
 * @param[in] code The code this command should execute
*/

#define DEF_JUMP_CMD(name, num, code)                                                   \
    case (num):                                                                         \
        {                                                                               \
            data->cmd[pos_cmd].command = num;                                           \
            if (command == (num + HAVE_ARG))                                            \
            {                                                                           \
                data->cmd[pos_cmd].argc = data->buf[++id];                              \
            }                                                                           \
            else                                                                        \
            {                                                                           \
                return ERR_COMMAND;                                                     \
            }                                                                           \
            break;                                                                      \
        }

/**
 * A function that divides the buffer into separate commands.
 * @param[in] data Structure containing all information
 * @param[out] code_error Code error
*/

int split_commands (TEXT *data)
{   
    my_assert (data != NULL);

    data->n_cmd = number_of_commands (data->buf, data->size_file);

    data->cmd = (COMMANDS *) calloc (data->n_cmd, sizeof (COMMANDS));

    size_t pos_cmd = 0;

    for (size_t id = 0; id < data->size_file; id++)
    {
        int command = data->buf[id];

        switch (command & 0x1F)
        {
            #include "..\include\commands.h"
            #include "..\include\jump_cmd.h"

            default:
                return ERR_COMMAND;
        }
        pos_cmd++;
    }

    return ERR_NO;
}

#undef DEF_CMD

#undef DEF_JUMP_CMD

/**
 * Function that counts the number of commands.
 * @param[in] data Structure containing all information
 * @param[in] n_cmd
*/

size_t number_of_commands (const int *data, const size_t size)
{
    my_assert (data != NULL);

    size_t n = 0;

    for (size_t i = 0; i < size; i++)
    {
        n++;
        
        if (data[i] == (PUSH + HAVE_ARG) || data[i] == (PUSH + HAVE_REG) || data[i] == (POP + HAVE_REG))
        {
            i++;
        }
        else if (data[i] == (JMP + HAVE_ARG) || data[i] == (JA + HAVE_ARG) || data[i] == (JAE + HAVE_ARG) || data[i] == (CALL + HAVE_ARG))
        {
            i++;
        }
        else if (data[i] == (JB + HAVE_ARG) || data[i] == (JBE + HAVE_ARG) || data[i] == (JE + HAVE_ARG) || data[i] == (JNE + HAVE_ARG))
        {
            i++;
        }
    }

    return n;
}

/**
 * Function returning file size.
 * @param[in] stream Pointer to file
 * @param[out] size_file File size
*/

size_t get_file_size (FILE *stream)
{
    my_assert (stream != NULL);

    size_t start = ftell (stream);
    fseek (stream, start, SEEK_END);
    size_t size_file = ftell (stream);
    rewind (stream);

    return size_file;
}

/**
 * Function that clears all variables.
 * @param[in] data Structure containing all information
*/

void text_free(TEXT *data)
{
    my_assert (data != NULL)

    free (data->buf);
    free (data->cmd);
    data->buf = NULL;
    data->cmd = NULL;

    data->fp_input = NULL;
    data->fp_print = NULL;

    data->n_cmd = VALUE_DEFAULT;
    data->size_file = VALUE_DEFAULT;
}
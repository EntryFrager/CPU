/// @file func.cpp

#include "func.h"

/**
 * Function that reads information from a file.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Code error
*/

int input_text (SPU* spu)
{
    my_assert (spu != NULL);

    spu->file_name_input = (const char *) "..\\ass_output.bin";

    spu->fp_input = fopen (spu->file_name_input, "r + b");

    if (spu->fp_input == NULL)
    {
        return ERR_FOPEN;
    }

    spu->size_file = get_file_size (spu->fp_input) / 4;

    spu->buf = (int *) calloc (spu->size_file * sizeof (int), sizeof (char));
    my_assert (spu->buf != NULL);

    size_t read_size = fread (spu->buf, sizeof (int), spu->size_file, spu->fp_input);

    if (read_size != spu->size_file)
    {
        return ERR_FREAD;
    }

    if (fclose (spu->fp_input) != 0)
    {
        return ERR_FCLOSE;
    }

    int code_error = split_commands (spu);

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
            spu->cmd[pos_cmd].command = command;                                        \
            if (have_param)                                                             \
            {                                                                           \
                if (command & HAVE_RAM)                                                 \
                {                                                                       \
                    spu->cmd[pos_cmd].ram = spu->buf[++ip];                             \
                }                                                                       \
                if (command & HAVE_ARG)                                                 \
                {                                                                       \
                    spu->cmd[pos_cmd].argc = spu->buf[++ip];                            \
                }                                                                       \
                else if (command & HAVE_REG)                                            \
                {                                                                       \
                    spu->cmd[pos_cmd].reg = spu->buf[++ip];                             \
                }                                                                       \
                else                                                                    \
                {                                                                       \
                    return ERR_COMMAND;                                                 \
                }                                                                       \
            }                                                                           \
            break;                                                                      \
        }

/**
 * A function that divides the buffer into separate commands.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int split_commands (SPU *spu)
{   
    my_assert (spu != NULL);

    spu->n_cmd = number_of_commands (spu->buf, spu->size_file);

    spu->cmd = (COMMANDS *) calloc (spu->n_cmd, sizeof (COMMANDS));

    size_t pos_cmd = 0;

    for (size_t ip = 0; ip < spu->size_file; ip++)
    {
        int command = spu->buf[ip];

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
 * @param[in] spu Structure containing all information
 * @param[in] n_cmd
*/

size_t number_of_commands (const int *data, const size_t size)
{
    my_assert (data != NULL);

    size_t n = 0;

    for (size_t i = 0; i < size; i++)
    {
        n++;

        if ((data[i] & HAVE_ARG) || (data[i] & HAVE_REG))
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
 * @param[in] spu Structure containing all information
*/

void spu_dtor(SPU *spu)
{
    my_assert (spu != NULL)

    stack_dtor (&spu->stack);
    stack_dtor (&spu->stack_call);

    free (spu->buf);
    free (spu->cmd);
    free (spu->reg_value);
    free (spu->ram_value);

    spu->buf       = NULL;
    spu->cmd       = NULL;
    spu->reg_value = NULL;
    spu->ram_value = NULL;

    spu->fp_input  = NULL;
    spu->fp_print  = NULL;

    spu->n_cmd     = VALUE_DEFAULT;
    spu->size_file = VALUE_DEFAULT;
}
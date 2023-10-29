/// @file func_ass.cpp

#include "func_ass.h"

/**
 * Function that counts the number of commands.
 * @param[in] spu Structure containing all information
*/

void number_of_commands (SPU *spu)
{
    my_assert (spu != NULL);

    for (size_t ip = 0; ip <= spu->size_file; ip++)
    {
        if ((spu->buf_input[ip] == '\n' || spu->buf_input[ip] == '\0') && spu->buf_input[ip - 2] != '\n')
        {
            spu->n_cmd++;
            spu->n_words++;
        }
        else if (spu->buf_input[ip] == ' ' && spu->buf_input[ip - 1] != ' ' && spu->buf_input[ip + 1] != ' ')
        {
            spu->n_words++;
        }
    }
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

void spu_dtor (SPU *spu)
{
    my_assert (spu != NULL)

    free (spu->buf_input);
    free (spu->cmd);
    free (spu->label);
    spu->buf_input = NULL;
    spu->cmd = NULL;
    spu->label = NULL;

    spu->fp_input = NULL;
    spu->fp_print_txt = NULL;
    spu->fp_print_bin = NULL;

    spu->n_cmd = VALUE_DEFAULT;
    spu->size_file = VALUE_DEFAULT;
}
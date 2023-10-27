/// @file func_ass.cpp

#include "func_ass.h"

/**
 * Function that counts the number of commands.
 * @param[in] data Structure containing all information
*/

void number_of_commands (TEXT *data)
{
    my_assert (data != NULL);

    for (size_t i = 2; i <= data->size_file; i++)
    {
        if ((data->buf[i] == '\n' && data->buf[i - 2] != '\n') || data->buf[i] == ' ')
        {
            data->n_words++;
        }
        else if (data->buf[i] == '\0' && data->buf[i - 2] != '\n')
        {
            data->n_words++;
        }

        if (data->buf[i] == '\n' && data->buf[i - 2] != '\n')
        {
            data->n_cmd++;
        }
        else if (data->buf[i] == '\0' && data->buf[i - 2] != '\n')
        {
            data->n_cmd++;
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
 * @param[in] data Structure containing all information
*/

void text_free(TEXT *data)
{
    my_assert (data != NULL)

    free (data->buf);
    free (data->cmd);
    free (data->label);
    data->buf = NULL;
    data->cmd = NULL;
    data->label = NULL;

    data->fp_input = NULL;
    data->fp_print_txt = NULL;
    data->fp_print_bin = NULL;

    data->n_cmd = VALUE_DEFAULT;
    data->size_file = VALUE_DEFAULT;
    data->n_words = VALUE_DEFAULT;
}
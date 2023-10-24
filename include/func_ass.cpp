#include "func_ass.h"

void number_of_commands (TEXT *data)
{
    my_assert (data != NULL);

    data->n_cmd++;
    data->n_words++;

    for (size_t i = 1; i < data->size_file; i++)
    {
        if ((data->buf[i] == '\n' && data->buf[i - 1] != '\n') || data->buf[i] == ' ')
        {
            data->n_words++;
        }

        if (data->buf[i] == '\n' && data->buf[i - 1] != '\n')
        {
            data->n_cmd++;
        }
    }
}

size_t get_file_size (FILE *stream)
{
    my_assert (stream != NULL);

    size_t start = ftell (stream);
    fseek (stream, start, SEEK_END);
    size_t size_file = ftell (stream);
    rewind (stream);

    return size_file;
}

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
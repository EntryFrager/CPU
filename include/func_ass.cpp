#include "func_ass.h"

void number_of_commands (TEXT *data)
{
    my_assert (data != NULL);

    data->n_comms++;
    data->n_words++;

    for (size_t i = 1; i < data->size_file; i++)
    {
        if ((data->buf[i] == '\n' && data->buf[i - 1] != '\n') || data->buf[i] == ' ')
        {
            data->n_words++;
        }

        if (data->buf[i] == '\n' && data->buf[i - 1] != '\n')
        {
            data->n_comms++;
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
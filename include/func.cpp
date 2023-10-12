#include "func.h"
#include "func_ass.h"

char *move_point (TEXT *data, const char *buf)
{
    my_assert (data != NULL);
    my_assert (buf != NULL);

    int len = strlen (buf);
    data->buf += len + 1;

    while (*data->buf == ' ' || *data->buf == '\n')
    {
        data->buf++;
    }   

    return data->buf;
}

size_t number_of_lines (const char *data, const size_t size)
{
    my_assert (data != NULL);

    size_t n = 1;

    for (size_t i = 1; i < size; i++)
    {
        if (data[i] == '\n' && data[i - 1] != '\n')
        {
            n++;
        }
    }

    return n;
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
#include "func_ass.h"

size_t number_of_commands (const char *data, const size_t size)
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
#include "ass.h"

static size_t number_of_lines (const char *data, const size_t size);

const char *COMMAND[] = {
    "hlt",
    "out",
    "push",
    "add",
    "sub",
    "mul",
    "div",
    "sqrt",
    "sin",
    "cos",
    "in"
};

const int COMMAND_CNT = 10;

int input_text (TEXT* data)
{
    my_assert (data != NULL);

    data->file_name_input = (const char *) "D:\\vscode\\calculator\\ass_input.txt";

    data->fp_input = fopen (data->file_name_input, "rb");

    if (data->fp_input == NULL)
    {
        return ERR_FOPEN;
    }

    data->size_file = get_file_size (data->fp_input);

    data->buf = (char *) calloc (data->size_file + 1, sizeof (char));
    my_assert (data->buf != NULL);

    size_t read_size = fread (data->buf, sizeof (char), data->size_file, data->fp_input);

    if (read_size != data->size_file)
    {
        return ERR_FREAD;
    }

    *(data->buf + data->size_file) = '\0';

    if (fclose (data->fp_input) != 0)
    {
        return ERR_FCLOSE;
    }

    split_lines (data);

    return ERR_NO;
}

void split_lines (TEXT *data)
{   
    my_assert (data != NULL);

    data->n_lines =  number_of_lines (data->buf, data->size_file);

    data->lines = (LINE *) calloc (data->n_lines, sizeof(LINE));
    my_assert (data->lines != NULL);

    (data->lines)[0].str = data->buf;
    (data->lines)[0].size_str = 1;
    int j = 1;

    for (size_t id = 1; id <= data->size_file; id++)
    {
        if (data->buf[id] == '\n' && data->buf[id - 1] != '\n')
        {
            *(data->buf + id) = '\0';
            
            (data->lines)[j].str = data->buf + (id + 1);

            j++;
        }
        else
        {
            (data->lines)[j - 1].size_str++;
        }
    }   
}

static size_t number_of_lines (const char *data, const size_t size)
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

void print_text (TEXT *data)
{
    for (size_t i = 0; i < data->n_lines; i++)
    {
        for (int j = 0; j < COMMAND_CNT; j++)
        {
            if (strncmp (data->lines[i].str, COMMAND[j], strlen (COMMAND[j])) == 0)
            {
                if (j == 2)
                {
                    fprintf (data->fp_print, "%d %s", j, (data->lines[i].str + 5));
                }
                else if ((i + 1) == data->n_lines)
                {
                    fprintf (data->fp_print, "%d", j);
                }
                else
                {
                    fprintf (data->fp_print, "%d\n", j);
                }
            }
        }
    }
}
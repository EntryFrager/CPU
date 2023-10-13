#include "func.h"
#include "func_ass.h"

int input_text (TEXT* data)
{
    my_assert (data != NULL);

    data->file_name_input = (const char *) "..\\include\\ass_output.bin";

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

int split_commands (TEXT *data)
{   
    my_assert (data != NULL);

    data->n_cmd = number_of_commands (data->buf, data->size_file);

    data->cmd = (COMMANDS *) calloc (data->n_cmd, sizeof (COMMANDS));

    size_t pos_cmd = 0;

    for (size_t id = 0; id < data->size_file; id++)
    {
        int command = data->buf[id];

        switch (command)
        {
            case HLT:
            {
                data->cmd[pos_cmd].command = command;
                
                break;
            }
            case OUT:
            {
                data->cmd[pos_cmd].command = command;
                
                break;
            }
            case (PUSH + (1 << 5)):
            {
                data->cmd[pos_cmd].command = command;
                data->cmd[pos_cmd].reg = data->buf[++id];
                
                break;
            }
            case (PUSH + (1 << 4)):
            {
                data->cmd[pos_cmd].command = command;
                data->cmd[pos_cmd].argc = data->buf[++id];
                
                break;
            }
            case (POP + (1 << 5)):
            {
                data->cmd[pos_cmd].command = PUSH + (1 << 5);
                data->cmd[pos_cmd].reg = data->buf[++id];
                
                break;
            }
            case POP:
            {
                data->cmd[pos_cmd].command = command;
                
                break;
            }
            case ADD:
            {
                data->cmd[pos_cmd].command = command;
                
                break;
            }
            case SUB:
            {
                data->cmd[pos_cmd].command = command;
                
                break;
            }
            case MUL:
            {
                data->cmd[pos_cmd].command = command;
                
                break;
            }
            case DIV:
            {
                data->cmd[pos_cmd].command = command;
                
                break;
            }
            case SIN:
            {
                data->cmd[pos_cmd].command = command;
                
                break;
            }
            case COS:
            {
                data->cmd[pos_cmd].command = command;
                
                break;
            }
            case SQRT:
            {
                data->cmd[pos_cmd].command = command;
                
                break;
            }
            case IN:
            {
                data->cmd[pos_cmd].command = command;
                
                break;
            }
            default:
                return ERR_COMMAND;
        }
        pos_cmd++;
    }

    return ERR_NO;
}

size_t number_of_commands (const int *data, const size_t size)
{
    my_assert (data != NULL);

    size_t n = 0;

    for (size_t i = 0; i < size; i++)
    {
        n++;
        if (data[i] == (PUSH + (1 << 4)) || data[i] == (PUSH + (1 << 5)) || data[i] == (POP + (1 << 5)) || data[i] == POP)
        {
            i++;
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
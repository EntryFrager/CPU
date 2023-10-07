#include "calc.h"

static size_t number_of_lines (const char *data, const size_t size);

int input_text (TEXT* data)
{
    my_assert (data != NULL);

    data->file_name_input = (const char *) "D:\\vscode\\calculator\\ass_output.txt";

    data->fp_input = fopen (data->file_name_input, "rb");

    if (data->fp_input == NULL)
    {
        return ERR_FOPEN;
    }

    data->size_file = get_file_size (data->fp_input);

    data->buf = (int *) calloc (data->size_file, sizeof (char));
    my_assert (data->buf != NULL);

    int a = 0;

    do {
        fscanf (data->fp_input, "%d", &a);

        data->buf[data->n_comms++] = a;
    } while (a != 0);

    if (fclose (data->fp_input) != 0)
    {
        return ERR_FCLOSE;
    }

    split_commands (data);

    return ERR_NO;
}

void split_commands (TEXT *data)
{   
    my_assert (data != NULL);

    data->commands = (COMMS *) calloc (data->n_comms, sizeof(COMMS));
    my_assert (data->commands != NULL);

    for (size_t i = 0; i < data->n_comms; i++)
    {
        data->commands[i].command = data->buf[i];

        if (data->buf[i] == 2)
        {
            data->commands[i].argc = data->buf[i++];
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

int calc_func (STACK *stack, TEXT *data)
{
    fprintf (data->fp_print, "Это программа, выполняющая небольшую часть функций калькулятора.\n");

    ELEMENT a = 0;
    ELEMENT b = 0;
    int value = 0;

    for (size_t i = 0; i < data->n_comms; i++)
    {
        switch (data->commands[i].command)
        {
            case HLT:
                fprintf (data->fp_print, "Расчёт окончен.\n");
                return 0;
            case OUT:
                fprintf (data->fp_print, "%d\n", stack_pop (stack));
                break;
            case PUSH:
                stack_push (stack, data->commands[i].argc);
                break;
            case ADD:
                a = stack_pop (stack);
                b = stack_pop (stack);

                stack_push (stack, a + b);
                break;
            case SUB:
                a = stack_pop (stack);
                b = stack_pop (stack);

                stack_push (stack, b - a);
                break;
            case MUL:
                a = stack_pop (stack);
                b = stack_pop (stack);

                stack_push (stack, b * a);
                break;
            case DIV:
                a = stack_pop (stack);
                b = stack_pop (stack);

                stack_push (stack, b / a);
                break;
            case SQRT:
                a = stack_pop (stack);

                stack_push (stack, (ELEMENT) sqrt (a));
                break;
            case SIN:
                a = stack_pop (stack);

                stack_push (stack, (ELEMENT) sin (a));
                break;
            case COS:
                a = stack_pop (stack);

                stack_push (stack, (ELEMENT) cos (a));
                break;
            case IN:
                scanf ("%d", &a);
                stack_push (stack, (ELEMENT) a);
                break;
        }
    }

    return 1;
}
#include "calc.h"

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

const int COMMAND_CNT = 11;

int input_text (TEXT* data)
{
    my_assert (data != NULL);

    data->file_name_input = (const char *) "..\\include\\ass_output.txt";

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

    split_commands (data);

    return ERR_NO;
}

void split_commands (TEXT *data)
{   
    my_assert (data != NULL);

    data->n_comms =  number_of_lines (data->buf, data->size_file);

    data->cmd = (COMMANDS *) calloc (data->n_comms, sizeof(COMMANDS));
    my_assert (data->cmd != NULL);

    char buf[256] = "";

    for (size_t id = 0; id < data->n_comms; id++)
    {
        sscanf (data->buf, "%d", &data->cmd[id].command);

        sprintf (buf, "%d", data->cmd[id].command);
        data->buf = move_point (data, buf);     

        if (data->cmd[id].command == PUSH)
        {
            sscanf (data->buf, "%d", &data->cmd[id].argc);

            sprintf (buf, "%d", data->cmd[id].argc);
            data->buf = move_point (data, buf);  
        }
        else
        {
            data->cmd[id].argc = -1;
        }
    }   
}

int calc_func (STACK *stack, TEXT *data)
{
    assert_stack (stack);
    my_assert (data != NULL);

    fprintf (data->fp_print, "Это программа, выполняющая небольшую часть функций калькулятора.\n");

    ELEMENT a = 0;
    ELEMENT b = 0;

    for (size_t id = 0; id < data->n_comms; id++)
    {
        switch (data->cmd[id].command)
        {
            case HLT:
                fprintf (data->fp_print, "Расчёт окончен.\n");
                return ERR_NO;
            case OUT:
                fprintf (data->fp_print, "Ответ: %d\n", stack_pop (stack));
                break;
            case PUSH:
                stack_push (stack, data->cmd[id].argc);
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
            default:
                return ERR_COMMAND;
        }
    }

    return ERR_NO;
}
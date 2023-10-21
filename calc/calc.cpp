#include "calc.h"

int REG_VALUE[REG_CNT] = {0};

#define DEF_PUSH(arg) stack_push (stack, arg)

#define DEF_POP stack_pop (stack)

#define DEF_CMD(name, num, have_arg, code)              \
    case (num):                                         \
        {                                               \
            code                                        \
            break;                                      \
        }

int calc_func (STACK *stack, TEXT *data)
{
    assert_stack (stack);
    my_assert (data != NULL);

    data->file_name_print = "..\\include\\result.txt";

    data->fp_print = fopen (data->file_name_print, "wb");

    if (data->fp_print == NULL)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FOPEN));
    }

    fprintf (data->fp_print, "Это программа, выполняющая небольшую часть функций калькулятора.\n");

    for (size_t id = 0; id < data->n_cmd; id++)
    {
        switch (data->cmd[id].command)
        {
            #include "..\include\commands.h"

            default:
                return ERR_COMMAND;
        }
    }

    if (fclose (data->fp_print) != 0)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FCLOSE));
    }

    return ERR_NO;
}

#undef DEF_CMD
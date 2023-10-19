#include "calc.h"

int REG_VALUE[REG_CNT] = {0};

#define DEF_CMD(name, mode, code_input, code_calc)              \
    case (name):                                                \
        code_calc

int calc_func (STACK *stack, TEXT *data)
{
    assert_stack (stack);
    my_assert (data != NULL);

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

    return ERR_NO;
}

#undef DEF_CMD
#include "calc.h"

int REG_VALUE[REG_CNT] = {0};

#define DEF_PUSH(stack_name, arg) stack_push (stack_name, arg)

#define DEF_POP(stack_name) stack_pop (stack_name)

#define DEF_CMD(name, num, have_arg, code)              \
    case (num):                                         \
        {                                               \
            ELEMENT a = VALUE_DEFAULT;                  \
            ELEMENT b = VALUE_DEFAULT;                  \
            code                                        \
            break;                                      \
        }

#define DEF_JUMP_CMD(name, num, code)                   \
    case (num):                                         \
        {                                               \
            code;                                       \
            break;                                      \
        }

int calc_func (STACK *stack, TEXT *data)
{
    assert_stack (stack);
    my_assert (data != NULL);

    STACK stack_call = {};
    stack_ctor (&stack_call, LABEL_CNT);

    int ram[SIZE_RAM] = {0};

    data->file_name_print = "..\\result.txt";

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
            #include "..\include\jump_cmd.h"

            default:
                return ERR_COMMAND;
        }

        for (size_t ram_pos = 0; ram_pos < SIZE_RAM; ram_pos++)
        {
            if (ram[ram_pos] == 0)
            {
                printf (".");
            }
            else
            {
                printf ("*");
            }
        }

        printf ("\n");
    }

    if (fclose (data->fp_print) != 0)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FCLOSE));
    }

    stack_dtor (&stack_call);

    return ERR_NO;
}

#undef DEF_CMD

#undef DEF_JUMP_CMD
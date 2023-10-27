/// file @calc.cpp

#include "calc.h"

int REG_VALUE[REG_CNT] = {0};                                                                                           ///< Array containing register values.

#define DEF_PUSH(stack_name, arg) stack_push (stack_name, arg)                                                          ///< Macro that adds an element to the stack.

#define DEF_POP(stack_name) stack_pop (stack_name)                                                                      ///< Macro that removes an element from the stack.

/**
 * Macro for code generation of commands
 * @param[in] name Сommand name
 * @param[in] num Command number
 * @param[in] have_arg The presence of argument
 * @param[in] code The code this command should execute
*/

#define DEF_CMD(name, num, have_arg, code)              \
    case (num):                                         \
        {                                               \
            ELEMENT a = VALUE_DEFAULT;                  \
            ELEMENT b = VALUE_DEFAULT;                  \
            code                                        \
            break;                                      \
        }

/**
 * Macro for code generation of commands like jump
 * @param[in] name Сommand name
 * @param[in] num Command number
 * @param[in] code The code this command should execute
*/


#define DEF_JUMP_CMD(name, num, code)                   \
    case (num):                                         \
        {                                               \
            code;                                       \
            break;                                      \
        }
    
/**
 * A function that performs all calculations and outputs them to a file.
 * @param[in] stack A structure that acts as memory in our processor
 * @param[in] data Structure containing all information
*/

int calc_func (STACK *stack, TEXT *data)
{
    assert_stack (stack);
    my_assert (data != NULL);

    STACK stack_call = {};
    stack_ctor (&stack_call, LABEL_CNT);

    int *ram = (int *) calloc (SIZE_RAM, sizeof (int));

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

        graph_video (ram);
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

/**
 * Function that prints RAM values.
 * @param[in] ram Array with RAM values
*/

void graph_video (int *ram)
{
    my_assert (ram != NULL);

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
/// file @calc.cpp

#include "spu.h"

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
            code                                        \
            break;                                      \
        }
    
/**
 * A function that performs all calculations and outputs them to a file.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int spu_ran (SPU *spu)
{
    my_assert (spu != NULL);

    fprintf (spu->fp_print, "This is a program that implements the functions of the processor.\n");

    int ip = 0;

    while (*(spu->buf + ip) != EOF)
    {
        ELEMENT a = VALUE_DEFAULT;
        ELEMENT b = VALUE_DEFAULT;

        switch (*(spu->buf + ip) & 0x1F)
        {
            #include "..\include\commands.h"
            #include "..\include\jump_cmd.h"

            default:
                return ERR_COMMAND;
        }

        ip++;
    }

    return ERR_NO;
}

#undef DEF_CMD

/**
 * Function that prints RAM values.
 * @param[in] ram Array with RAM values
*/

void graph_video (ELEMENT *ram)
{
    my_assert (ram != NULL);

    for (size_t ram_pos = 0; ram_pos < SIZE_RAM; ram_pos++)
    {
        if (ram[ram_pos] == 0)
        {
            printf ("\x1b[30m.\x1b[0m");
        }
        else
        {
            printf ("\x1b[31m*\x1b[0m");
        }
    }

    printf ("\n");
}
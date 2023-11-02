/// file @calc.cpp

#include "spu.h"

#define PUSH(stack_name, arg) stack_push (stack_name, arg)                                                              ///< Macro that adds an element to the stack.

#define POP(stack_name) stack_pop (stack_name)                                                                          ///< Macro that removes an element from the stack.

/**
 * Function to initialize the spu structure.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int spu_ctor (SPU *spu)
{
    my_assert (spu != NULL);

    stack_ctor (&spu->stack, STACK_DEFAULT_SIZE);
    stack_ctor (&spu->stack_call, LABEL_CNT);

    spu->file_name_input = (const char *) "..\\ass_output.bin";
    spu->file_name_print = "..\\result.txt";

    spu->fp_input = fopen (spu->file_name_input, "r + b");
    spu->fp_print = fopen (spu->file_name_print, "wb");

    if (spu->fp_print == NULL || spu->fp_input == NULL)
    {
        return ERR_FOPEN;
    }

    spu->ram_value = (ELEMENT *) calloc (SIZE_RAM, sizeof (ELEMENT));
    my_assert (spu->ram_value != NULL);

    spu->reg_value = (ELEMENT *) calloc (REG_CNT , sizeof (ELEMENT));
    my_assert (spu->reg_value != NULL);

    return ERR_NO;
}

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

    for (size_t ip = 0; ip <= 275; ip++)
    {
        ELEMENT a = VALUE_DEFAULT;
        ELEMENT b = VALUE_DEFAULT;

        switch (spu->buf[ip] & 0x1F)
        {
            #include "..\include\commands.h"
            #include "..\include\jump_cmd.h"

            default:
                return ERR_COMMAND;
        }

        CHECK_BUF_IP (ip)
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

/**
 * Function that clears all variables.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int spu_dtor(SPU *spu)
{
    my_assert (spu != NULL)

    if (fclose (spu->fp_print) != 0 || fclose (spu->fp_input) != 0)
    {
        return ERR_FCLOSE;
    }

    stack_dtor (&spu->stack);
    stack_dtor (&spu->stack_call);

    free (spu->buf);
    free (spu->reg_value);
    free (spu->ram_value);

    spu->buf       = NULL;
    spu->reg_value = NULL;
    spu->ram_value = NULL;

    spu->fp_input  = NULL;
    spu->fp_print  = NULL;

    spu->size_file = VALUE_DEFAULT;

    return ERR_NO;
}

#undef CHECK_BUF_IP
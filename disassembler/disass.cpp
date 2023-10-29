#include "disass.h"

/**
 * Array wuth registers name.
*/

const char *REG[] = {
    "rax",
    "rbx",
    "rcx",
    "rdx"
};

/**
 * Macro for code generation of commands
 * @param[in] name Сommand name
 * @param[in] num Command number
 * @param[in] have_arg The presence of argument
 * @param[code] code 
*/

#define DEF_CMD(name, num, have_param, code)                                                                            \
    case (num):                                                                                                         \
        {                                                                                                               \
            if (have_param)                                                                                             \
            {                                                                                                           \
                if (spu->cmd[ip].command & HAVE_RAM)                                                                    \
                {                                                                                                       \
                    if (spu->cmd[ip].command & HAVE_REG)                                                                \
                    {                                                                                                   \
                        fprintf (spu->fp_print, "%s [%s]\n", name, REG[spu->cmd[ip].reg - 1]);                          \
                    }                                                                                                   \
                    else                                                                                                \
                    {                                                                                                   \
                        fprintf (spu->fp_print, "%s [%d]\n", name, spu->cmd[ip].argc);                                  \
                    }                                                                                                   \
                }                                                                                                       \
                else if (spu->cmd[ip].command & HAVE_REG)                                                               \
                {                                                                                                       \
                    fprintf (spu->fp_print, "%s %s\n", name, REG[spu->cmd[ip].reg - 1]);                                \
                }                                                                                                       \
                else if (spu->cmd[ip].command & HAVE_ARG)                                                               \
                {                                                                                                       \
                    fprintf (spu->fp_print, "%s %d\n", name, spu->cmd[ip].argc);                                        \
                }                                                                                                       \
            }                                                                                                           \
            else                                                                                                        \
            {                                                                                                           \
                fprintf (spu->fp_print, "%s\n", name);                                                                  \
            }                                                                                                           \
            break;                                                                                                      \
        }

/**
 * A function that outputs machine code to a new file.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int print_text (SPU *spu)
{
    my_assert (spu != NULL);
    
    for (size_t ip = 0; ip < spu->n_cmd; ip++)
    {
        switch (spu->cmd[ip].command & 0x1F)
        {
            #include "..\include\commands.h"
            #include "..\include\jump_cmd.h"

            default:
                return ERR_COMMAND;
        }
    }

    return ERR_NO;
}

#undef DEF_CMD
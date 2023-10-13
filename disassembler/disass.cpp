#include "disass.h"

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
    "in",
    "pop"
};

const char *REG[] = {
    "rax",
    "rbx",
    "rcx",
    "rdx"
};

void print_text (TEXT *data)
{
    my_assert (data != NULL);
    
    for (size_t id = 0; id < data->n_cmd; id++)
    {
        switch (data->cmd[id].command)
        {
            case (PUSH + (1 << 5)):
            {
                fprintf (data->fp_print, "%s %s\n", COMMAND[PUSH], REG[data->cmd[id].reg - 1]);
                
                break;
            }
            case (PUSH + (1 << 4)):
            {
                fprintf (data->fp_print, "%s %d\n", COMMAND[PUSH], (data->cmd[id].argc));
                
                break;
            }
            case (POP + (1 << 5)):
            {
                fprintf (data->fp_print, "%s %s\n", COMMAND[POP], REG[data->cmd[id].reg - 1]);
                
                break;
            }
            case (HLT):
            {
                fprintf (data->fp_print, "%s", COMMAND[data->cmd[id].command]);
                
                break;
            }
            default:
            {
                fprintf (data->fp_print, "%s\n", COMMAND[data->cmd[id].command]);
                
                break;
            }
        }
    }
}
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

#define DEF_CMD(name, code)                     \
    case (name):                                \
        code

void print_text (TEXT *data)
{
    my_assert (data != NULL);
    
    for (size_t id = 0; id < data->n_cmd; id++)
    {
        switch (data->cmd[id].command)
        {
            #include "commands.h"

            default:
            {
                fprintf (data->fp_print, "%s\n", COMMAND[data->cmd[id].command]);
                
                break;
            }
        }
    }
}

#undef DEF_CMD
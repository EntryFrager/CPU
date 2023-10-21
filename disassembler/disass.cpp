#include "disass.h"

const char *REG[] = {
    "rax",
    "rbx",
    "rcx",
    "rdx"
};

#define DEF_CMD(name, num, have_param, code)                                                                            \
    case (num):                                                                                                         \
        {                                                                                                               \
            if (have_param)                                                                                             \
            {                                                                                                           \
                if (data->cmd[id].reg != VALUE_DEFAULT)                                                                 \
                {                                                                                                       \
                    fprintf (data->fp_print, "%s %s\n", name, REG[data->cmd[id].reg - 1]);                              \
                }                                                                                                       \
                else                                                                                                    \
                {                                                                                                       \
                    fprintf (data->fp_print, "%s %d\n", name, data->cmd[id].argc);                                      \
                }                                                                                                       \
            }                                                                                                           \
            else                                                                                                        \
            {                                                                                                           \
                if (num == 0)                                                                                           \
                {                                                                                                       \
                    fprintf (data->fp_print, "%s", name);                                                               \
                }                                                                                                       \
                else                                                                                                    \
                {                                                                                                       \
                    fprintf (data->fp_print, "%s\n", name);                                                             \
                }                                                                                                       \
            }                                                                                                           \
            break;                                                                                                      \
        }

int print_text (TEXT *data)
{
    my_assert (data != NULL);
    
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
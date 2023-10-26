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
                if (data->cmd[id].ram != VALUE_DEFAULT && data->cmd[id].reg != VALUE_DEFAULT)                           \
                {                                                                                                       \
                    fprintf (data->fp_print, "%s [%s]\n", name, REG[data->cmd[id].reg - 1]);                            \
                }                                                                                                       \
                else if (data->cmd[id].ram != VALUE_DEFAULT)                                                            \
                {                                                                                                       \
                    fprintf (data->fp_print, "%s [%d]\n", name, data->cmd[id].argc);                                    \
                }                                                                                                       \
                else if (data->cmd[id].reg != VALUE_DEFAULT)                                                            \
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
                if (id == data->n_cmd - 1)                                                                              \
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
    
#define DEF_JUMP_CMD(name, num, code)                                                                                   \
    case (num):                                                                                                         \
        {                                                                                                               \
            if (data->cmd[id].reg != VALUE_DEFAULT)                                                                     \
            {                                                                                                           \
                fprintf (data->fp_print, "%s %s\n", name, REG[data->cmd[id].reg - 1]);                                  \
            }                                                                                                           \
            else                                                                                                        \
            {                                                                                                           \
                fprintf (data->fp_print, "%s %d\n", name, data->cmd[id].argc);                                          \
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
            #include "..\include\jump_cmd.h"

            default:
                return ERR_COMMAND;
        }
    }

    return ERR_NO;
}

#undef DEF_CMD
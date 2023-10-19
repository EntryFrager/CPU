DEF_CMD (HLT, false, 
    {
        data->cmd[pos_cmd].command = command;
        break;
    },
    {
        fprintf (data->fp_print, "Расчёт окончен.\n");
        return ERR_NO;
    })

DEF_CMD (OUT, false,
    {
        data->cmd[pos_cmd].command = command;
        break;
    },
    {
        fprintf (data->fp_print, "Ответ: %lf\n", stack->data[stack->position - 1]);
        break;
    })

DEF_CMD (PUSH + (1 << 4), true, 
    {
        data->cmd[pos_cmd].command = command;
        data->cmd[pos_cmd].argc = data->buf[++id];
        break;
    },
    {
        stack_push (stack, data->cmd[id].argc);
        break;
    })

DEF_CMD (PUSH + (1 << 5), true, 
    {
        data->cmd[pos_cmd].command = command;
        data->cmd[pos_cmd].reg = data->buf[++id];
        break;
    },
    {
        stack_push (stack, REG_VALUE[data->cmd[id].reg - 1]);
        break;
    })

DEF_CMD (POP + (1 << 5), true,
    {
        data->cmd[pos_cmd].command = POP + (1 << 5);
        data->cmd[pos_cmd].reg = data->buf[++id];
        break;
    },
    {
        REG_VALUE[data->cmd[id].reg - 1] = stack_pop (stack);
        break;
    })

DEF_CMD (ADD, false, 
    {
        data->cmd[pos_cmd].command = command;   \
        break;
    },
    {
        stack_push (stack, stack_pop (stack) + stack_pop (stack));
        break;
    })

DEF_CMD (SUB, false, 
    {
        data->cmd[pos_cmd].command = command;   \
        break;
    },
    {
        stack_push (stack, - stack_pop (stack) + stack_pop (stack));
        break;
    })

DEF_CMD (MUL, false, 
    {
        data->cmd[pos_cmd].command = command;   \
        break;
    },
    {
        stack_push (stack, stack_pop (stack) * stack_pop (stack));
        break;
    })

DEF_CMD (DIV, false, 
    {
        data->cmd[pos_cmd].command = command;   \
        break;
    },
    {
        stack_push (stack, (1 / stack_pop (stack)) * stack_pop (stack));
        break;
    })

DEF_CMD (SIN, false, 
    {
        data->cmd[pos_cmd].command = command;   \
        break;
    },
    {
        stack_push (stack, (ELEMENT) sin (stack_pop (stack)));
        break;
    })

DEF_CMD (COS, false, 
    {
        data->cmd[pos_cmd].command = command;   \
        break;
    },
    {
        stack_push (stack, (ELEMENT) cos (stack_pop (stack)));
        break;
    })

DEF_CMD (SQRT, false, 
    {
        data->cmd[pos_cmd].command = command;   \
        break;
    },
    {
        stack_push (stack, (ELEMENT) sqrt (stack_pop (stack)));
        break;
    })

DEF_CMD (IN, false, 
    {
        data->cmd[pos_cmd].command = command;   \
        break;
    },
    {
        ELEMENT value = 0;
        scanf ("%lf", &value);
        stack_push (stack, (ELEMENT) value);
        break;
    })

DEF_CMD (POP, false, 
    {
        data->cmd[pos_cmd].command = command;   \
        break;
    },
    {
        stack_pop (stack);
        break;
    })
DEF_CMD ("hlt", HLT, false,
    {
        fprintf (data->fp_print, "Расчёт окончен.");
        return ERR_NO;
    })

DEF_CMD ("out", OUT, false,
    {
        printf ("%lld\n", stack->data[stack->position - 1]);
        fprintf (data->fp_print, "Ответ: %lld\n", stack->data[stack->position - 1]);
    })

DEF_CMD ("push", PUSH, true,
    {
        if (data->cmd[id].ram != VALUE_DEFAULT && data->cmd[id].reg != VALUE_DEFAULT)
        {
            if (REG_VALUE[data->cmd[id].reg - 1] > 99 || REG_VALUE[data->cmd[id].reg - 1] < 0)
            {
                return ERR_RAM;
            }

            DEF_PUSH (stack, ram[REG_VALUE[data->cmd[id].reg - 1]]);
        }
        else if (data->cmd[id].ram != VALUE_DEFAULT)
        {
            if (data->cmd[id].argc > 99 || data->cmd[id].argc < 0)
            {
                return ERR_RAM;
            }

            DEF_PUSH (stack, ram[data->cmd[id].argc]);
        }
        else if (data->cmd[id].reg != VALUE_DEFAULT)
        {
            DEF_PUSH (stack, REG_VALUE[data->cmd[id].reg - 1]);
        }
        else
        {
            DEF_PUSH (stack, data->cmd[id].argc);
        }
    })

DEF_CMD ("add", ADD, false,
    {
        a = DEF_POP (stack);
        b = DEF_POP (stack);

        DEF_PUSH (stack, a + b);
    })

DEF_CMD ("sub", SUB, false,
    {
        a = DEF_POP (stack);
        b = DEF_POP (stack);

        DEF_PUSH (stack, b - a);
    })

DEF_CMD ("mul", MUL, false,
    {
        a = DEF_POP (stack);
        b = DEF_POP (stack);

        DEF_PUSH (stack, a * b);
    })

DEF_CMD ("div", DIV, false,
    {
        a = DEF_POP (stack);
        b = DEF_POP (stack);

        DEF_PUSH (stack, b / a);
    })

DEF_CMD ("sin", SIN, false,
    {
        a = DEF_POP (stack);

        DEF_PUSH (stack, (ELEMENT) sin (a));
    })

DEF_CMD ("cos", COS, false,
    {
        a = DEF_POP (stack);

        DEF_PUSH (stack, (ELEMENT) cos (a));
    })

DEF_CMD ("sqrt", SQRT, false,
    {
        a = DEF_POP (stack);

        DEF_PUSH (stack, (ELEMENT) sqrt (a));
    })

DEF_CMD ("in", IN, false,
    {
        ELEMENT value = 0;

        if (scanf ("%lf", &value) != 1)
        {
            return ERR_INPUT_ARG;
        }

        DEF_PUSH (stack, (ELEMENT) value);
    })

DEF_CMD ("pop", POP, true,
    {
        if (data->cmd[id].ram != VALUE_DEFAULT && data->cmd[id].reg != VALUE_DEFAULT)
        {
            if (REG_VALUE[data->cmd[id].reg - 1] > 99 || REG_VALUE[data->cmd[id].reg - 1] < 0)
            {
                return ERR_RAM;
            }

            ram[REG_VALUE[data->cmd[id].reg - 1]] = DEF_POP (stack);
        }
        else if (data->cmd[id].ram != VALUE_DEFAULT)
        {
            if (data->cmd[id].argc > 99 || data->cmd[id].argc < 0)
            {
                return ERR_RAM;
            }

            ram[data->cmd[id].argc] = DEF_POP (stack);
        }
        else
        {
            REG_VALUE[data->cmd[id].reg - 1] = DEF_POP (stack);
        }
    })

DEF_CMD ("ret", RET, false,
    {
        a = DEF_POP (&stack_call);
        id = a;
    })
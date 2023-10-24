DEF_CMD ("hlt", HLT, false,
    {
        fprintf (data->fp_print, "Расчёт окончен.\n");
        return ERR_NO;
    })

DEF_CMD ("out", OUT, false,
    {
        fprintf (data->fp_print, "Ответ: %lf\n", stack->data[stack->position - 1]);
    })

DEF_CMD ("push", PUSH, true,
    {
        if (data->cmd[id].reg != VALUE_DEFAULT)
        {
            DEF_PUSH (REG_VALUE[data->cmd[id].reg - 1]);
        }
        else
        {
            DEF_PUSH (data->cmd[id].argc);
        }
    })

DEF_CMD ("add", ADD, false,
    {
        a = DEF_POP;
        b = DEF_POP;

        DEF_PUSH (a + b);
    })

DEF_CMD ("sub", SUB, false,
    {
        a = DEF_POP;
        b = DEF_POP;

        DEF_PUSH (b - a);
    })

DEF_CMD ("mul", MUL, false,
    {
        a = DEF_POP;
        b = DEF_POP;

        DEF_PUSH (a * b);
    })

DEF_CMD ("div", DIV, false,
    {
        a = DEF_POP;
        b = DEF_POP;

        DEF_PUSH (b / a);
    })

DEF_CMD ("sin", SIN, false,
    {
        a = DEF_POP;

        DEF_PUSH ((ELEMENT) sin (a));
    })

DEF_CMD ("cos", COS, false,
    {
        a = DEF_POP;

        DEF_PUSH ((ELEMENT) cos (a));
    })

DEF_CMD ("sqrt", SQRT, false,
    {
        a = DEF_POP;

        DEF_PUSH ((ELEMENT) sqrt (a));
    })

DEF_CMD ("in", IN, false,
    {
        ELEMENT value = 0;
        scanf ("%lf", &value);
        DEF_PUSH ((ELEMENT) value);
    })

DEF_CMD ("pop", POP, true,
    {
        REG_VALUE[data->cmd[id].reg - 1] = DEF_POP;
    })

DEF_JUMP_CMD ("jmp", JMP,
    {
        id = data->cmd[id].argc - 1;
    })

DEF_JUMP_CMD ("ja", JA,
    {
        if (stack->data[stack->position - 1] > stack->data[stack->position - 2])
        {
            id = data->cmd[id].argc - 1;
        }
    })

DEF_JUMP_CMD ("jae", JAE,
    {
        if (stack->data[stack->position - 1] >= stack->data[stack->position - 2])
        {
            id = data->cmd[id].argc - 1;
        }
    })

DEF_JUMP_CMD ("jb", JB,
    {
        if (stack->data[stack->position - 1] < stack->data[stack->position - 2])
        {
            id = data->cmd[id].argc - 1;
        }
    })

DEF_JUMP_CMD ("jBE", JBE,
    {
        if (stack->data[stack->position - 1] <= stack->data[stack->position - 2])
        {
            id = data->cmd[id].argc - 1;
        }
    })

DEF_JUMP_CMD ("jE", JE,
    {
        if (stack->data[stack->position - 1] == stack->data[stack->position - 2])
        {
            id = data->cmd[id].argc - 1;
        }
    })

DEF_JUMP_CMD ("jne", JNE,
    {
        if (stack->data[stack->position - 1] != stack->data[stack->position - 2])
        {
            id = data->cmd[id].argc - 1;
        }
    })
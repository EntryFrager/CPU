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
        DEF_PUSH (DEF_POP + DEF_POP);
    })

DEF_CMD ("sub", SUB, false,
    {
        ELEMENT a = DEF_POP;
        ELEMENT b = DEF_POP;

        DEF_PUSH (b - a);
    })

DEF_CMD ("mul", MUL, false,
    {
        DEF_PUSH (DEF_POP * DEF_POP);
    })

DEF_CMD ("div", DIV, false,
    {
        DEF_PUSH ((1 / DEF_POP) * DEF_POP);
    })

DEF_CMD ("sin", SIN, false,
    {
        DEF_PUSH ((ELEMENT) sin (DEF_POP));
    })

DEF_CMD ("cos", COS, false,
    {
        DEF_PUSH ((ELEMENT) cos (DEF_POP));
    })

DEF_CMD ("sqrt", SQRT, false,
    {
        DEF_PUSH ((ELEMENT) sqrt (DEF_POP));
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
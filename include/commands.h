/// @file commands.h

/**
 * The hlt command that stops the program.
*/

DEF_CMD("hlt", HLT, false,
    {
        fprintf (spu->fp_print, "Расчёт окончен.");
        return ERR_NO;
    })

DEF_CMD("outc", OUTC, true,
    {
        char *outc = (char *) calloc (spu->cmd[ip].argc + 1, sizeof (char));
        my_assert (outc != NULL);

        for (size_t i = 0; i < spu->cmd[ip].argc; i++)
        {
            *(outc + spu->cmd[ip].argc - i - 1) = (int) DEF_POP(&spu->stack);
        }

        *(outc + spu->cmd[ip].argc) = '\0';
        
        fprintf (spu->fp_print, "%s\n", outc);

        free (outc);
        outc = NULL;
    })

/**
 * The command that prints the response.
*/

DEF_CMD("out", OUT, false,
    {
        fprintf (spu->fp_print, "Ответ: %lf\n", spu->stack.data[spu->stack.position - 1]);
    })

/**
 * Command that adds an element to memory (stack).
*/

DEF_CMD("push", PUSH, true,
    {
        if (spu->cmd[ip].command & HAVE_RAM)
        {
            if (spu->cmd[ip].command & HAVE_REG)
            {
                if (spu->reg_value[spu->cmd[ip].reg - 1] > 99 || spu->reg_value[spu->cmd[ip].reg - 1] < 0)
                {
                    return ERR_RAM;
                }

                DEF_PUSH (&spu->stack, spu->ram_value[(size_t) spu->reg_value[spu->cmd[ip].reg - 1]]);

                spu->ram_value[(size_t) spu->reg_value[spu->cmd[ip].reg - 1]] = 0;
            }
            else
            {
                if (spu->cmd[ip].argc > 99 || spu->cmd[ip].argc < 0)
                {
                    return ERR_RAM;
                }

                DEF_PUSH (&spu->stack, spu->ram_value[spu->cmd[ip].argc]);

                spu->ram_value[spu->cmd[ip].argc] = 0;
            }
        }
        else if (spu->cmd[ip].command & HAVE_REG)
        {
            DEF_PUSH (&spu->stack, spu->reg_value[spu->cmd[ip].reg - 1]);
        }
        else if (spu->cmd[ip].command & HAVE_ARG)
        {
            DEF_PUSH (&spu->stack, spu->cmd[ip].argc);
        }
    })

/**
 * Command that removes an element from memory (stack).
*/

DEF_CMD("pop", POP, true,
    {
        if (spu->cmd[ip].command & HAVE_RAM)
        {
            if (spu->cmd[ip].command & HAVE_REG)
            {
                if (spu->reg_value[spu->cmd[ip].reg - 1] > 99 || spu->reg_value[spu->cmd[ip].reg - 1] < 0)
                {
                    return ERR_RAM;
                }

                spu->ram_value[(size_t) spu->reg_value[spu->cmd[ip].reg - 1]] = DEF_POP (&spu->stack);
            }
            else
            {
                if (spu->cmd[ip].argc > 99 || spu->cmd[ip].argc < 0)
                {
                    return ERR_RAM;
                }

                spu->ram_value[(size_t) spu->cmd[ip].argc] = DEF_POP (&spu->stack);
            }
        }
        else if (spu->cmd[ip].command & HAVE_REG)
        {
            spu->reg_value[spu->cmd[ip].reg - 1] = DEF_POP (&spu->stack);
        }
        else
        {
            return ERR_ARGC;
        }
    })

/**
 * Addition command.
*/

DEF_CMD("add", ADD, false,
    {
        a = DEF_POP (&spu->stack);
        b = DEF_POP (&spu->stack);

        DEF_PUSH (&spu->stack, a + b);
    })

/**
 * Subtraction command.
*/

DEF_CMD("sub", SUB, false,
    {
        a = DEF_POP (&spu->stack);
        b = DEF_POP (&spu->stack);

        DEF_PUSH (&spu->stack, b - a);
    })

/**
 * Multiply command.
*/

DEF_CMD("mul", MUL, false,
    {
        a = DEF_POP (&spu->stack);
        b = DEF_POP (&spu->stack);

        DEF_PUSH (&spu->stack, a * b);
    })

/**
 * Division command.
*/

DEF_CMD("div", DIV, false,
    {
        a = DEF_POP (&spu->stack);
        b = DEF_POP (&spu->stack);
        
        DEF_PUSH (&spu->stack, b / a);
    })

/**
 * Sine command.
*/

DEF_CMD("sin", SIN, false,
    {
        a = DEF_POP (&spu->stack);

        DEF_PUSH (&spu->stack, (ELEMENT) sin (a));
    })

/**
 * Cosine command.
*/

DEF_CMD("cos", COS, false,
    {
        a = DEF_POP (&spu->stack);

        DEF_PUSH (&spu->stack, (ELEMENT) cos (a));
    })

/**
 * Root command.
*/

DEF_CMD("sqrt", SQRT, false,
    {
        a = DEF_POP (&spu->stack);

        DEF_PUSH (&spu->stack, (ELEMENT) sqrt (a));
    })

/**
 * A command that allows the user to enter a number using an input device.
*/

DEF_CMD("in", IN, false,
    {
        ELEMENT value = 0;

        if (scanf ("%lf", &value) != 1)
        {
            return ERR_INPUT_ARG;
        }

        DEF_PUSH (&spu->stack, (ELEMENT) value);
    })

/**
 * Return command to label call command.
*/

DEF_CMD("ret", RET, false,
    {
        a = DEF_POP (&spu->stack_call);
        ip = (size_t) a;
    })

/**
 * Command that starts graphics memory.
*/

DEF_CMD("draw", DRAW, false,
    {
        graph_video (spu->ram_value);
    })
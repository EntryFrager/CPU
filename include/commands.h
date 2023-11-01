/// @file commands.h

/**
 * The hlt command that stops the program.
*/

DEF_CMD("hlt", HLT, false,
    {
        fprintf (spu->fp_print, "Расчёт окончен.");
        return ERR_NO;
    })

/**
 * This command prints characters whose acsii codes are stored on the stack.
*/

DEF_CMD("outc", OUTC, true,
    {
        ip++;
        char *outc = (char *) calloc (*(spu->buf + ip) + 1, sizeof (char));
        my_assert (outc != NULL);

        for (size_t i = 0; i < (size_t) *(spu->buf + ip); i++)
        {
            *(outc + *(spu->buf + ip) - i - 1) = (int) DEF_POP(&spu->stack);
        }

        *(outc + *(spu->buf + ip)) = '\0';
        
        fprintf (spu->fp_print, "%s\n", outc);

        free (outc);
        outc = NULL;
    })

/**
 * The command that prints the response.
*/

DEF_CMD("out", OUT, false,
    {
        fprintf (spu->fp_print, "Answer: %lf\n", spu->stack.data[spu->stack.position - 1]);
    })

/**
 * Command that adds an element to memory (stack).
*/

DEF_CMD("push", PUSH, true,
    {
        if (*(spu->buf + ip) & HAVE_RAM)
        {
            if (*(spu->buf + ip) & HAVE_REG)
            {
                ip++;
                if (spu->reg_value[*(spu->buf + ip) - 1] > 99 || spu->reg_value[*(spu->buf + ip) - 1] < 0)
                {
                    return ERR_RAM;
                }

                DEF_PUSH (&spu->stack, spu->ram_value[(size_t) spu->reg_value[*(spu->buf + ip) - 1]]);

                spu->ram_value[(size_t) spu->reg_value[*(spu->buf + ip) - 1]] = 0;
            }
            else
            {
                ip++;
                if (*(spu->buf + ip) > 99 || *(spu->buf + ip) < 0)
                {
                    return ERR_RAM;
                }

                DEF_PUSH (&spu->stack, spu->ram_value[*(spu->buf + ip)]);

                spu->ram_value[*(spu->buf + ip)] = 0;
            }
        }
        else if (*(spu->buf + ip) & HAVE_REG)
        {
            ip++;
            DEF_PUSH (&spu->stack, spu->reg_value[*(spu->buf + ip) - 1]);
        }
        else if (*(spu->buf + ip) & HAVE_ARG)
        {
            ip++;
            DEF_PUSH (&spu->stack, *(spu->buf + ip));
        }
        else
        {
            return ERR_ARGC;
        }
    })

/**
 * Command that removes an element from memory (stack).
*/

DEF_CMD("pop", POP, true,
    {
        if (*(spu->buf + ip) & HAVE_RAM)
        {
            if (*(spu->buf + ip) & HAVE_REG)
            {
                ip++;
                if (spu->reg_value[*(spu->buf + ip) - 1] > 99 || spu->reg_value[*(spu->buf + ip) - 1] < 0)
                {
                    return ERR_RAM;
                }

                spu->ram_value[(size_t) spu->reg_value[*(spu->buf + ip) - 1]] = DEF_POP (&spu->stack);
            }
            else
            {
                ip++;
                if (*(spu->buf + ip) > 99 || *(spu->buf + ip) < 0)
                {
                    return ERR_RAM;
                }

                spu->ram_value[(size_t) *(spu->buf + ip)] = DEF_POP (&spu->stack);
            }
        }
        else if (*(spu->buf + ip) & HAVE_REG)
        {
            ip++;
            spu->reg_value[*(spu->buf + ip) - 1] = DEF_POP (&spu->stack);
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
        DEF_PUSH (&spu->stack, (ELEMENT) sin (DEF_POP (&spu->stack)));
    })

/**
 * Cosine command.
*/

DEF_CMD("cos", COS, false,
    {
        DEF_PUSH (&spu->stack, (ELEMENT) cos (DEF_POP (&spu->stack)));
    })

/**
 * Root command.
*/

DEF_CMD("sqrt", SQRT, false,
    {
        DEF_PUSH (&spu->stack, (ELEMENT) sqrt (DEF_POP (&spu->stack)));
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
 * Command that starts graphics memory.
*/

DEF_CMD("draw", DRAW, false,
    {
        graph_video (spu->ram_value);
    })
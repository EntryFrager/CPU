/// @file commands.h

#define CHECK_RAM_IP(ram_ip) if (ram_ip > 99 || ram_ip < 0) return ERR_RAM;

#define GET_ARGUMENT(code_ram_reg, code_ram, code_reg, code_argc)   \
    if ((spu->buf[ip] & HAVE_RAM) && (spu->buf[ip] & HAVE_REG))     \
    {                                                               \
        ip++;                                                       \
        CHECK_BUF_IP(ip)                                            \
        CHECK_RAM_IP (spu->reg_value[spu->buf[ip]]);                \
        code_ram_reg                                                \
    }                                                               \
    else if (spu->buf[ip] & HAVE_RAM)                               \
    {                                                               \
        ip++;                                                       \
        CHECK_BUF_IP(ip)                                            \
        CHECK_RAM_IP (spu->buf[ip]);                                \
        code_ram                                                    \
    }                                                               \
    else if (spu->buf[ip] & HAVE_REG)                               \
    {                                                               \
        ip++;                                                       \
        CHECK_BUF_IP(ip)                                            \
        code_reg                                                    \
    }                                                               \
    else if (spu->buf[ip] & HAVE_ARG)                               \
    {                                                               \
        ip++;                                                       \
        CHECK_BUF_IP(ip)                                            \
        code_argc                                                   \
    }                                                               \
    else                                                            \
    {                                                               \
        return ERR_ARGC;                                            \
    }


/**
 * The hlt command that stops the program.
*/

DEF_CMD("hlt", HLT, false,
    {
        fprintf (spu->fp_print, "Расчёт окончен.");
        return ERR_NO;
    })

/**
 * This function takes as input the number of characters that need to be displayed on the screen. 
 * The ASCII codes of these characters must be added to the top of the stack in advance
*/

DEF_CMD("outc", OUTC, true,
    {
        char *outc = (char *) calloc (spu->buf[++ip] + 1, sizeof (char));
        my_assert (outc != NULL);

        for (size_t i = 0; i < (size_t) spu->buf[ip]; i++)
        {
            *(outc + spu->buf[ip] - i - 1) = (int) POP(&spu->stack);
        }

        *(outc + spu->buf[ip]) = '\0';
        
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
        GET_ARGUMENT (
            {
                PUSH (&spu->stack, spu->ram_value[(size_t) spu->reg_value[spu->buf[ip] - 1]]);
                spu->ram_value[(size_t) spu->reg_value[(size_t) spu->buf[ip] - 1]] = 0;
            },
            {
                PUSH (&spu->stack, spu->ram_value[spu->buf[ip]]);
                spu->ram_value[(size_t) spu->buf[ip]] = 0;
            },
            {
                PUSH (&spu->stack, spu->reg_value[spu->buf[ip] - 1]);
            },
            {
                PUSH (&spu->stack, spu->buf[ip]);
            })
    })

/**
 * Command that removes an element from memory (stack).
*/

DEF_CMD("pop", POP, true,
    {
        GET_ARGUMENT (
            {
                spu->ram_value[(size_t) spu->reg_value[(size_t) spu->buf[ip] - 1]] = POP (&spu->stack);
            },
            {
                spu->ram_value[(size_t) spu->buf[ip]] = POP (&spu->stack);
            },
            {
                spu->reg_value[(size_t) spu->buf[ip] - 1] = POP (&spu->stack);
            },
            {
                return ERR_ARGC;
            })
    })

/**
 * Addition command.
*/

DEF_CMD("add", ADD, false,
    {
        a = POP (&spu->stack);
        b = POP (&spu->stack);

        PUSH (&spu->stack, a + b);
    })

/**
 * Subtraction command.
*/

DEF_CMD("sub", SUB, false,
    {
        a = POP (&spu->stack);
        b = POP (&spu->stack);

        PUSH (&spu->stack, b - a);
    })

/**
 * Multiply command.
*/

DEF_CMD("mul", MUL, false,
    {
        a = POP (&spu->stack);
        b = POP (&spu->stack);

        PUSH (&spu->stack, a * b);
    })

/**
 * Division command.
*/

DEF_CMD("div", DIV, false,
    {
        a = POP (&spu->stack);
        b = POP (&spu->stack);

        PUSH (&spu->stack, b / a);
    })

/**
 * Sine command.
*/

DEF_CMD("sin", SIN, false,
    {
        PUSH (&spu->stack, (ELEMENT) sin (POP (&spu->stack)));
    })

/**
 * Cosine command.
*/

DEF_CMD("cos", COS, false,
    {
        PUSH (&spu->stack, (ELEMENT) cos (POP (&spu->stack)));
    })

/**
 * Root command.
*/

DEF_CMD("sqrt", SQRT, false,
    {
        PUSH (&spu->stack, (ELEMENT) sqrt (POP (&spu->stack)));
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

        PUSH (&spu->stack, (ELEMENT) value);
    })

/**
 * Return command to label call command.
*/

DEF_CMD("ret", RET, false,
    {
        ip = (size_t) POP (&spu->stack_call);
    })

/**
 * Command that starts graphics memory.
*/

DEF_CMD("draw", DRAW, false,
    {
        graph_video (spu->ram_value);
    })

#undef GET_ARGUMENT
#undef CHECK_RAM_IP
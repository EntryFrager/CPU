/// @file commands.h

/**
 * The hlt command that stops the program.
*/

DEF_CMD ("hlt", HLT, false,
    {
        fprintf (data->fp_print, "Расчёт окончен.");
        return ERR_NO;
    })

/**
 * The command that prints the response.
*/

DEF_CMD ("out", OUT, false,
    {
        fprintf (data->fp_print, "Ответ: %llf\n", stack->data[stack->position - 1]);
    })

/**
 * Command that adds an element to memory (stack).
*/

DEF_CMD ("push", PUSH, true,
    {
        if (data->cmd[id].ram != VALUE_DEFAULT && data->cmd[id].reg != VALUE_DEFAULT)
        {
            if (REG_VALUE[data->cmd[id].reg - 1] > 99 || REG_VALUE[data->cmd[id].reg - 1] < 0)
            {
                return ERR_RAM;
            }

            DEF_PUSH (stack, ram[REG_VALUE[data->cmd[id].reg - 1]]);

            ram[REG_VALUE[data->cmd[id].reg - 1]] = 0;
        }
        else if (data->cmd[id].ram != VALUE_DEFAULT)
        {
            if (data->cmd[id].argc > 99 || data->cmd[id].argc < 0)
            {
                return ERR_RAM;
            }

            DEF_PUSH (stack, ram[data->cmd[id].argc]);

            ram[data->cmd[id].argc] = 0;
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

/**
 * Command that removes an element from memory (stack).
*/

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

/**
 * Addition command.
*/

DEF_CMD ("add", ADD, false,
    {
        a = DEF_POP (stack);
        b = DEF_POP (stack);

        DEF_PUSH (stack, a + b);
    })

/**
 * Subtraction command.
*/

DEF_CMD ("sub", SUB, false,
    {
        a = DEF_POP (stack);
        b = DEF_POP (stack);

        DEF_PUSH (stack, b - a);
    })

/**
 * Multiply command.
*/

DEF_CMD ("mul", MUL, false,
    {
        a = DEF_POP (stack);
        b = DEF_POP (stack);

        DEF_PUSH (stack, a * b);
    })

/**
 * Division command.
*/

DEF_CMD ("div", DIV, false,
    {
        a = DEF_POP (stack);
        b = DEF_POP (stack);

        DEF_PUSH (stack, b / a);
    })

/**
 * Sine command.
*/

DEF_CMD ("sin", SIN, false,
    {
        a = DEF_POP (stack);

        DEF_PUSH (stack, (ELEMENT) sin (a));
    })

/**
 * Cosine command.
*/

DEF_CMD ("cos", COS, false,
    {
        a = DEF_POP (stack);

        DEF_PUSH (stack, (ELEMENT) cos (a));
    })

/**
 * Root command.
*/

DEF_CMD ("sqrt", SQRT, false,
    {
        a = DEF_POP (stack);

        DEF_PUSH (stack, (ELEMENT) sqrt (a));
    })

/**
 * A command that allows the user to enter a number using an input device.
*/

DEF_CMD ("in", IN, false,
    {
        ELEMENT value = 0;

        if (scanf ("%lf", &value) != 1)
        {
            return ERR_INPUT_ARG;
        }

        DEF_PUSH (stack, (ELEMENT) value);
    })

/**
 * Return command to label call command.
*/

DEF_CMD ("ret", RET, false,
    {
        a = DEF_POP (&stack_call);
        id = a;
    })
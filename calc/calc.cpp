#include "calc.h"

int REG_VALUE[REG_CNT] = {0};

int calc_func (STACK *stack, TEXT *data)
{
    assert_stack (stack);
    my_assert (data != NULL);

    fprintf (data->fp_print, "Это программа, выполняющая небольшую часть функций калькулятора.\n");

    for (size_t id = 0; id < data->n_cmd; id++)
    {
        switch (data->cmd[id].command)
        {
            case HLT:
            {
                fprintf (data->fp_print, "Расчёт окончен.\n");

                return ERR_NO;
            }
            case OUT:
            {
                fprintf (data->fp_print, "Ответ: %lf\n", stack->data[stack->position - 1]);

                break;
            }
            case (PUSH + (1 << 4)):
            {
                stack_push (stack, data->cmd[id].argc);

                break;
            }
            case (PUSH + (1 << 5)):
            {
                stack_push (stack, REG_VALUE[data->cmd[id].reg - 1]);

                break;
            }
            case ADD:
            {
                stack_push (stack, stack_pop (stack) + stack_pop (stack));

                break;
            }
            case SUB:
            {
                stack_push (stack, - stack_pop (stack) + stack_pop (stack));

                break;
            }
            case MUL:
            {
                stack_push (stack, stack_pop (stack) * stack_pop (stack));

                break;
            }
            case DIV:
            {
                stack_push (stack, (1 / stack_pop (stack)) * stack_pop (stack));

                break;
            }
            case SQRT:
            {
                stack_push (stack, (ELEMENT) sqrt (stack_pop (stack)));

                break;
            }
            case SIN:
            {
                stack_push (stack, (ELEMENT) sin (stack_pop (stack)));

                break;
            }
            case COS:
            {
                stack_push (stack, (ELEMENT) cos (stack_pop (stack)));

                break;
            }
            case IN:
            {
                ELEMENT value = 0;
                scanf ("%lf", &value);
                stack_push (stack, (ELEMENT) value);

                break;
            }
            case POP:
            {
                stack_pop (stack);

                break;
            }
            case (POP + (1 << 5)):
            {
                REG_VALUE[data->cmd[id].reg - 1] = stack_pop (stack);

                break;
            }
            default:
                return ERR_COMMAND;
        }
    }

    return ERR_NO;
}
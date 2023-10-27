/// @file jump_cmd.h

/**
 * Call command that preserves the previous position.
*/

DEF_JUMP_CMD ("call", CALL,
    {
        DEF_PUSH (&stack_call, id);
        id = data->cmd[id].argc - 1;
    })

/**
 * Unconditional jump command.
*/

DEF_JUMP_CMD ("jmp", JMP,
    {
        id = data->cmd[id].argc - 1;
    })

/**
 * Jump command if the last number written to the stack is greater than the second to last number written to the stack.
*/

DEF_JUMP_CMD ("ja", JA,
    {
        if (stack->data[stack->position - 1] > stack->data[stack->position - 2])
        {
            id = data->cmd[id].argc - 1;
        }
    })

/**
 * Jump command if the last number written to the stack is greater than or equal to the second to last number written to the stack.
*/

DEF_JUMP_CMD ("jae", JAE,
    {
        if (stack->data[stack->position - 1] >= stack->data[stack->position - 2])
        {
            id = data->cmd[id].argc - 1;
        }
    })

/**
 * Jump command if the last number written to the stack is less than the second to last number written to the stack.
*/

DEF_JUMP_CMD ("jb", JB,
    {
        if (stack->data[stack->position - 1] < stack->data[stack->position - 2])
        {
            id = data->cmd[id].argc - 1;
        }
    })

/**
 * Jump command if the last number written to the stack is less than or equal to the second to last number written to the stack.
*/

DEF_JUMP_CMD ("jbe", JBE,
    {
        if (stack->data[stack->position - 1] <= stack->data[stack->position - 2])
        {
            id = data->cmd[id].argc - 1;
        }
    })

/**
 * Jump command if the last number written to the stack is equal to the second to last number written to the stack.
*/

DEF_JUMP_CMD ("je", JE,
    {
        if (stack->data[stack->position - 1] == stack->data[stack->position - 2])
        {
            id = data->cmd[id].argc - 1;
        }
    })

/**
 * Jump command if the last number written to the stack is not equal to the second to last number written to the stack.
*/

DEF_JUMP_CMD ("jne", JNE,
    {
        if (stack->data[stack->position - 1] != stack->data[stack->position - 2])
        {
            id = data->cmd[id].argc - 1;
        }
    })
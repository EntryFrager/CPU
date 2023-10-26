DEF_JUMP_CMD ("call", CALL,
    {
        DEF_PUSH (&stack_call, id);
        id = data->cmd[id].argc - 1;
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
/// @file jump_cmd.h

#define jump_condition(expr)                                                                            \
    if (spu->stack.data[spu->stack.position - 1] expr spu->stack.data[spu->stack.position - 2])         \
    {                                                                                                   \
        ip = (size_t) spu->cmd[ip].argc - 1;                                                            \
    }

/**
 * Call command that preserves the previous position.
*/

DEF_CMD("call", CALL, 1,
    {
        DEF_PUSH (&spu->stack_call, ip);
        ip = (size_t) spu->cmd[ip].argc - 1;
    })

/**
 * Unconditional jump command.
*/

DEF_CMD("jmp", JMP, 1,
    {
        ip =  (size_t) spu->cmd[ip].argc - 1;
    })

/**
 * Jump command if the last number written to the stack is greater than the second to last number written to the stack.
*/

DEF_CMD("ja", JA, 1,
    {
        jump_condition (>);
    })

/**
 * Jump command if the last number written to the stack is greater than or equal to the second to last number written to the stack.
*/

DEF_CMD("jae", JAE, 1,
    {
        jump_condition (>=);
    })

/**
 * Jump command if the last number written to the stack is less than the second to last number written to the stack.
*/

DEF_CMD("jb", JB, 1,
    {
        jump_condition (<);
    })

/**
 * Jump command if the last number written to the stack is less than or equal to the second to last number written to the stack.
*/

DEF_CMD("jbe", JBE, 1,
    {
        jump_condition (<=);
    })

/**
 * Jump command if the last number written to the stack is equal to the second to last number written to the stack.
*/

DEF_CMD("je", JE, 1,
    {
        jump_condition (==);
    })

/**
 * Jump command if the last number written to the stack is not equal to the second to last number written to the stack.
*/

DEF_CMD("jne", JNE, 1,
    {
        jump_condition (!=);
    })
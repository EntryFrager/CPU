#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "..\include\error.h"

#define PUSH(arg) stack_push (&stack, arg)

#define POP() stack_pop (&stack)

enum COMMANDS_CODE {
    HLT,
    OUT,
    PUSH,
    ADD,
    SUB,
    MUL,
    DIV,
    SQRT,
    SIN,
    COS,
    IN,
    POP
};

const size_t COMMAND_CNT = 12;

enum REG {
    RAX = 1,
    RBX,
    RCX,
    RDX
};

const size_t REG_CNT = 4;

const int COMMANDS_DEFAULT = 0;

typedef struct {
    int command = COMMANDS_DEFAULT;
    int argc = COMMANDS_DEFAULT;
    int reg = COMMANDS_DEFAULT;
} COMMANDS;

typedef struct {
    const char *file_name_input = NULL;
    const char *file_name_print = NULL;

    FILE *fp_input = NULL;
    FILE *fp_print = NULL;

    int *buf = NULL;

    size_t size_file = 0;
    size_t n_cmd = 0;

    COMMANDS *cmd = NULL;
} TEXT;

int input_text (TEXT *data);

int split_commands (TEXT *data);

size_t number_of_commands (const int *data, const size_t size);

size_t get_file_size (FILE *stream);

#endif //FUNC_H
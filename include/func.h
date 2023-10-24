#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "..\include\error.h"

enum COMMANDS_CODE {
    HLT,
    OUT,
    PUSH,
    POP,
    JMP,
    JA,
    JAE,
    JB,
    JBE,
    JE,
    JNE,
    ADD,
    SUB,
    MUL,
    DIV,
    SQRT,
    SIN,
    COS,
    IN,
};

const size_t COMMAND_CNT = 19;

enum REG {
    RAX = 1,
    RBX,
    RCX,
    RDX
};

const size_t REG_CNT = 4;

const int VALUE_DEFAULT = 0;

const int HAVE_REG = 1 << 6;
const int HAVE_ARG = 1 << 5;

typedef struct {
    int command = VALUE_DEFAULT;
    int argc = VALUE_DEFAULT;
    int reg = VALUE_DEFAULT;
} COMMANDS;

typedef struct {
    const char *file_name_input = NULL;
    const char *file_name_print = NULL;

    FILE *fp_input = NULL;
    FILE *fp_print = NULL;

    int *buf = NULL;

    size_t size_file = VALUE_DEFAULT;
    size_t n_cmd = VALUE_DEFAULT;

    COMMANDS *cmd = NULL;
} TEXT;

int input_text (TEXT *data);

int split_commands (TEXT *data);

size_t number_of_commands (const int *data, const size_t size);

size_t get_file_size (FILE *stream);

void text_free (TEXT *data);

#endif //FUNC_H
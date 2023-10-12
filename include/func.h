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

enum REG {
    RAX = 1,
    RBX,
    RCX,
    RDX
};

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

    char *buf = NULL;

    size_t size_file = 0;
    size_t n_comms = 0;

    COMMANDS *cmd = NULL;
} TEXT;

size_t number_of_lines (const char *data, const size_t size);

char *move_point (TEXT *data, const char *buf);

size_t get_file_size (FILE *stream);

#endif //FUNC_H
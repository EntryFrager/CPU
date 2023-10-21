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
    POP, 
};

const size_t COMMAND_CNT = 12;

enum REG {
    RAX = 1,
    RBX,
    RCX,
    RDX
};

const size_t REG_CNT = 4;

const int HAVE_REG = 1 << 5;
const int HAVE_ARG = 1 << 4;

const int VALUE_DEFAULT = 0;

typedef struct {
    size_t size_str = VALUE_DEFAULT;
    char *command = NULL;
    char *param = NULL;
    int argc = VALUE_DEFAULT;
    int reg = VALUE_DEFAULT;
} COMMANDS;

typedef struct {
    const char *file_name_input = NULL;
    const char *file_name_print_txt = NULL;
    const char *file_name_print_bin = NULL;

    FILE *fp_input = NULL;
    FILE *fp_print_txt = NULL;
    FILE *fp_print_bin = NULL;

    char *buf = NULL;

    size_t size_file = VALUE_DEFAULT;
    size_t n_comms = VALUE_DEFAULT;
    size_t n_words = VALUE_DEFAULT;

    COMMANDS *cmd = NULL;
} TEXT;

void number_of_commands (TEXT *data);

char *move_point (TEXT *data, const char *buf);

size_t get_file_size (FILE *stream);

#endif //FUNC_H
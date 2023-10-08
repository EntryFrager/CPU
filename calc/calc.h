#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>  

#include "stack.h"
#include "D:\vscode\calculator\error.h"

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
    IN
};

const int COMMAND_CNT = 11;

typedef struct {
    int command = 0;
    int argc = 0;
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

int input_text (TEXT *data);

void split_commands (TEXT *data);

size_t get_file_size (FILE *stream);

int calc_func (STACK *stack, TEXT *data);

#endif
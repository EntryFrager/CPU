#ifndef ASS_H
#define ASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "D:\vscode\calculator\error.h"

typedef struct {
    size_t size_str = 0;
    char *command = NULL;
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

int print_text (TEXT *data);

size_t get_file_size (FILE *stream);

#endif //ASS_H
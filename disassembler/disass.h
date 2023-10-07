#ifndef DISASS_H
#define DISASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "D:\vscode\calculator\error.h"

const int CODE_CNT = 10;

typedef struct {
    size_t size_str = 0;
    char *str = NULL;
} LINE;

typedef struct {
    const char *file_name_input = NULL;
    const char *file_name_print = NULL;

    FILE *fp_input = NULL;
    FILE *fp_print = NULL;

    char *buf = NULL;

    size_t size_file = 0;
    size_t n_lines = 0;

    LINE *lines = NULL;
} TEXT;

int input_text (TEXT *data);

void split_lines (TEXT *data);

void print_text (TEXT *data);

size_t get_file_size (FILE *stream);

#endif //DISASS_H
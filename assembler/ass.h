#ifndef ASS_H
#define ASS_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#include "..\include\error.h"
#include "..\include\func_ass.h"

int input_text (TEXT *data);

void split_commands (TEXT *data);

int print_text (TEXT *data);

int get_param (COMMANDS *cmd, char *cmd_str);

#endif //ASS_H
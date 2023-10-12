#ifndef DISASS_H
#define DISASS_H

#include "..\include\error.h"
#include "..\include\func.h"

int input_text (TEXT *data);

int split_commands (TEXT *data);

void print_text (TEXT *data);

#endif //DISASS_H
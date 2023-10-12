#ifndef CALC_H
#define CALC_H

#include "..\include\stack.h"
#include "..\include\error.h"
#include "..\include\func.h"

int input_text (TEXT *data);

int split_commands (TEXT *data);

int calc_func (STACK *stack, TEXT *data);

#endif
/// @file calc.h

#ifndef CALC_H
#define CALC_H

#include "..\include\stack.h"                                                           ///< Connects the file that stores the stack
#include "..\include\error.h"                                                           ///< Connects a file that displays errors.
#include "..\include\func.h"                                                            ///< Connects a file that reads text from a file.

int calc_func (STACK *stack, TEXT *data);                                               ///< A function that performs all calculations and outputs them to a file.

void graph_video (int *ram);                                                            ///< Function that prints RAM values.

#endif
/// @file calc.h

#ifndef CALC_H
#define CALC_H

#include "..\include\func.h"                                                            ///< Connects a file that reads text from a file.

int calc_func (SPU *spu);                                                               ///< A function that performs all calculations and outputs them to a file.

void graph_video (ELEMENT *ram);                                                        ///< Function that prints RAM values.

#endif
/// @file calc.h

#ifndef CALC_H
#define CALC_H

#include "..\include\func.h"                                                            ///< Connects a file that reads text from a file.

int spu_ctor (SPU *spu);                                                                ///< Function to initialize the spu structure.

int spu_ran (SPU *spu);                                                                 ///< A function that performs all calculations and outputs them to a file.

void graph_video (ELEMENT *ram);                                                        ///< Function that prints RAM values.

int spu_dtor (SPU *spu);                                                                ///< Function that clears all variables.

#endif
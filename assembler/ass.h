/// @file ass.h

#ifndef ASS_H
#define ASS_H

#include "..\include\func_ass.h"                                                                    ///< Connects a file that reads text from a file.

int input_text (SPU *spu);                                                                          ///< A function that reads text from a file into one buffer.

void split_commands (SPU *spu);                                                                     ///< Function that splits a buffer into lines.

void clean_comment (SPU *spu);                                                                      ///< Function that removes comments.

int compare_command (SPU *spu);                                                                     ///< A function that compiles user code into machine code and outputs it to a file.

int param_check (COMMANDS *cmd, LABELS *label, int cmd_len);                                       ///< A function that checks whether a command has a parameter.

int get_param (COMMANDS *cmd, LABELS *label, int cmd_len, int len);                                ///< Function that finds a parameter for a command.

#endif //ASS_H
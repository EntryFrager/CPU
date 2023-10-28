/// @file ass.h

#ifndef ASS_H
#define ASS_H

#include "..\include\func_ass.h"                                                                    ///< Connects a file that reads text from a file.

int input_text (SPU *proc);                                                                         ///< A function that reads text from a file into one buffer.

void split_commands (SPU *proc);                                                                    ///< Function that splits a buffer into lines.

int compare_command (SPU *proc);

int param_check (COMMANDS *cmd, LABELS **label, int cmd_len);

int get_param (COMMANDS *cmd, LABELS **label, int cmd_len, int len);

#endif //ASS_H
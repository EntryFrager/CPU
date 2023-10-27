/// @file ass.h

#ifndef ASS_H
#define ASS_H

#include "..\include\func_ass.h"                                                                    ///< Connects a file that reads text from a file.

int input_text (TEXT *data);                                                                        ///< A function that reads text from a file into one buffer.

void split_commands (TEXT *data);                                                                   ///< Function that splits a buffer into lines.

int print_text (TEXT *data);                                                                        ///< A function that outputs machine code to a new file.

int get_param (COMMANDS *cmd, char *cmd_str);                                                       ///< A function that detects whether a command has an argument or not.

#endif //ASS_H
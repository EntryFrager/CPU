/// @file func_ass.h

#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

#include "..\include\error.h"                                                                   ///< Connects a file that displays errors.

enum COMMANDS_CODE {                                                                            ///< All command codes.
    HLT,                                                                                        ///< The hlt command that stops the program.
    OUT,                                                                                        ///< The command that prints the response.
    PUSH,                                                                                       ///< Command that adds an element to memory (stack).
    POP,                                                                                        ///< Command that removes an element from memory (stack).
    CALL,                                                                                       ///< Call command that preserves the previous position.
    RET,                                                                                        ///< Return command to label call command.
    JMP,                                                                                        ///< Unconditional jump command.
    JA,                                                                                         ///< Jump command if the last number written to the stack is greater than the second to last number written to the stack.
    JAE,                                                                                        ///< Jump command if the last number written to the stack is greater than or equal to the second to last number written to the stack.
    JB,                                                                                         ///< Jump command if the last number written to the stack is less than the second to last number written to the stack.
    JBE,                                                                                        ///< Jump command if the last number written to the stack is less than or equal to the second to last number written to the stack.
    JE,                                                                                         ///< Jump command if the last number written to the stack is equal to the second to last number written to the stack.
    JNE,                                                                                        ///< Jump command if the last number written to the stack is not equal to the second to last number written to the stack.
    ADD,                                                                                        ///< Addition command.
    SUB,                                                                                        ///< Subtraction command.
    MUL,                                                                                        ///< Multiply command.
    DIV,                                                                                        ///< Division command.
    SQRT,                                                                                       ///< Sine command.
    SIN,                                                                                        ///< Cosine command.
    COS,                                                                                        ///< Root command.
    IN,                                                                                         ///< A command that allows the user to enter a number using an input device.
    DRAW,                                                                                       ///< Command that starts graphics memory.
};

const size_t COMMAND_CNT = 22;                                                                  ///< Number of commands.

enum REG {                                                                                      ///< Register codes.
    RAX = 1,
    RBX,
    RCX,
    RDX
};

const size_t REG_CNT = 4;                                                                       ///< Number of registers.

const size_t LABEL_CNT = 10;                                                                    ///< Maximum number of tags.

const int HAVE_RAM = 1 << 7;                                                                    ///< Code for a command interacting with RAM.
const int HAVE_REG = 1 << 6;                                                                    ///< Code for a command that interacts with a register.
const int HAVE_ARG = 1 << 5;                                                                    ///< Code for the command that interacts with the argument.

const int VALUE_DEFAULT = 0;                                                                    ///< Default value of variables.

typedef struct {                                                                                ///< Structure containing information about labels.
    int label_n_str = VALUE_DEFAULT;                                                            ///< Line number with label.
    char *name_label = NULL;                                                                    ///< Label name.
    int size_label = VALUE_DEFAULT;
} LABELS;

typedef struct {                                                                                ///< Structure containing information about commands.
    size_t size_str = VALUE_DEFAULT;                                                            ///< Command line size.
    char *command = NULL;                                                                       ///< Command line.
    int cmd_code = 0;
    int argc = VALUE_DEFAULT;                                                                   ///< Argument value.
    int reg = VALUE_DEFAULT;                                                                    ///< Register value.
    int ram = VALUE_DEFAULT;                                                                    ///< 1 if RAM is in use, 0 if not.
} COMMANDS;

typedef struct {                                                                                ///< Structure containing all the information necessary for the program to work.
    const char *file_name_input = NULL;                                                         ///< File name to enter.
    const char *file_name_print_txt = NULL;                                                     ///< File name of txt type for output.
    const char *file_name_print_bin = NULL;                                                     ///< File name of bin type for output.

    FILE *fp_input = NULL;                                                                      ///< Pointer to input file.
    FILE *fp_print_txt = NULL;                                                                  ///< Pointer to output file of type txt.
    FILE *fp_print_bin = NULL;                                                                  ///< Pointer to output file of type bin.

    char *buf_input = NULL;                                                                     ///< A buffer containing all the information read from the file.

    size_t size_file = VALUE_DEFAULT;                                                           ///< File size.
    size_t n_cmd = VALUE_DEFAULT;                                                               ///< Number of commands.
    size_t n_words= VALUE_DEFAULT;                                                              ///< Word count.

    COMMANDS *cmd = NULL;                                                                       ///< An array of structures storing information about commands.
    LABELS *label = NULL;                                                                       ///< An array of structures storing information about labels.
} SPU;

void number_of_commands (SPU *spu);                                                             ///< Function that counts the number of commands.

size_t get_file_size (FILE *stream);                                                            ///< Function returning file size.

void spu_dtor (SPU *spu);                                                                       ///< Function that clears all variables.

#endif //FUNC_H
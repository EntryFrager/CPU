/// @file main_ass.cpp

#include "ass.h"

/**
 * This program converts your code into machine code that is more understandable for the computer.
*/

int main (int argc, char *argv[])
{
    SPU spu = {};

    int code_error = 0;

    if (argc == 2)
    {
        spu.file_name_input = (const char *) argv[1];
    }

    if ((code_error = input_text (&spu)) != 0)
    {
        fprintf (stderr, "%s", my_strerr (code_error));
    }

    spu.file_name_print_bin = "..\\ass_output.bin";
    spu.file_name_print_txt = "..\\ass_output.log";

    if ((code_error = compare_command (&spu)) != 0)
    {
        fprintf (stderr, "%s", my_strerr (code_error));
    }

    spu_dtor (&spu);

    return 0;
}
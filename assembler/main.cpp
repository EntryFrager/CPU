/// @file main_ass.cpp

#include "ass.h"

/**
 * This program converts your code into machine code that is more understandable for the computer.
*/

int main (int argc, char *argv[])
{
    SPU spu = {};

    int code_error = spu_ctor (&spu, argc, argv);
    CHECK_ERROR_PRINT(code_error)

    code_error = input_text (&spu);
    CHECK_ERROR_PRINT (code_error)

    for (size_t ip = 0; ip < spu.n_cmd; ip++)
    {
        printf ("%s\n", spu.cmd[ip].command);
    }

    /*code_error = pars_command (&spu);
    CHECK_ERROR_PRINT (code_error)*/

    code_error = spu_dtor (&spu);
    CHECK_ERROR_PRINT (code_error)

    return 0;
}
/// @file main_spu.cpp

#include "spu.h"

/**
 * A program that performs actions that have been previously converted by assembler into machine code.
*/

int main ()
{
    SPU spu = {};

    int code_error = input_text (&spu);

    if (code_error != 0)
    {
        fprintf(stderr, "%s", my_strerr (code_error));
    }

    if ((code_error = calc_func (&spu)) != ERR_NO)
    {
        fprintf (stderr, "%s", my_strerr (code_error));
    }

    spu_dtor (&spu);

    return 0;
}
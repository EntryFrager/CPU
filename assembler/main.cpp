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

    code_error = pars_command (&spu, 1);
    CHECK_ERROR_PRINT (code_error)

#define SECOND_COMPIL

    code_error = pars_command (&spu, 2);
    CHECK_ERROR_PRINT (code_error)

#undef SECOND_COMPIL

    fwrite (spu.buf_output, sizeof (int), spu.n_words, spu.fp_print_bin);

    code_error = spu_dtor (&spu);
    CHECK_ERROR_PRINT (code_error)

    return 0;
}
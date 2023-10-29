/// @file main_disass.cpp

#include "disass.h"

/**
 * This is a program that converts machine code into plain, user-readable code.
*/

int main ()
{
    SPU spu = {};

    int code_error = input_text (&spu);

    if (code_error != 0)
    {
        fprintf(stderr, "%s", my_strerr (code_error));

        return 0;
    }

    spu.file_name_print = "..\\ass_input.txt";

    spu.fp_print = fopen (spu.file_name_print, "w");

    if (spu.fp_print == NULL)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FOPEN));
        
        return 0;
    }

    print_text (&spu);

    if (fclose (spu.fp_print) != 0)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FCLOSE));
        
        return 0;
    }

    return 0;
}
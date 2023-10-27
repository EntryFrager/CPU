/// @file main.cpp

#include "ass.h"

/**
 * This program converts your code into machine code that is more understandable for the computer.
*/

int main ()
{
    TEXT data = {};

    int code_error = input_text (&data);

    if (code_error != 0)
    {
        fprintf(stderr, "%s", my_strerr (code_error));
    }

    data.file_name_print_bin = "..\\ass_output.bin";
    data.file_name_print_txt = "..\\ass_output.txt";

    data.fp_print_bin = fopen (data.file_name_print_bin, "w + b");
    data.fp_print_txt = fopen (data.file_name_print_txt, "w");

    printf ("%s %d\n", data.cmd[0].command, data.cmd[0].size_str);
    printf ("%s %d\n", data.cmd[1].command, data.cmd[1].size_str);
    //printf ("%s %d\n", data.cmd[2].command, data.cmd[2].size_str);

    if (data.fp_print_txt == NULL || data.fp_print_bin == NULL)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FOPEN));
    }

    if ((code_error = print_text(&data)) != 0)
    {
        fprintf (stderr, "%s", my_strerr (code_error));
    }

    if (fclose (data.fp_print_txt) != 0 || fclose (data.fp_print_bin) != 0)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FCLOSE));
    }

    text_free (&data);

    return 0;
}
#include "ass.h"

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

    return 0;
}
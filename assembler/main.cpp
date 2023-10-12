#include "ass.h"

int main ()
{
    TEXT data = {};

    int code_error = input_text (&data);

    if (code_error != 0)
    {
        fprintf(stderr, "%s", my_strerr (code_error));
    }

    data.file_name_print = "..\\include\\ass_output.txt";

    data.fp_print = fopen (data.file_name_print, "w");

    if (data.fp_print == NULL)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FOPEN));
    }

    if ((code_error = print_text(&data)) != 0)
    {
        fprintf (stderr, "%s", my_strerr (code_error));
    }

    if (fclose (data.fp_print) != 0)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FCLOSE));
    }

    return 0;
}
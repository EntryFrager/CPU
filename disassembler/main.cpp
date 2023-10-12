#include "disass.h"

int main ()
{
    TEXT data = {};

    int code_error = input_text (&data);

    if (code_error != 0)
    {
        fprintf(stderr, "%s", my_strerr (code_error));

        return 0;
    }

    data.file_name_print = "..\\include\\ass_input.txt";

    data.fp_print = fopen (data.file_name_print, "w");

    if (data.fp_print == NULL)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FOPEN));
        
        return 0;
    }

    print_text (&data);

    if (fclose (data.fp_print) != 0)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FCLOSE));
        
        return 0;
    }

    return 0;
}
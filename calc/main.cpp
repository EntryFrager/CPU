#include "calc.h"

int main ()
{
    TEXT data = {};
    STACK stack = {};

    stack_ctor (&stack, 5);

    int code_error = input_text (&data);

    if (code_error != 0)
    {
        fprintf(stderr, "%s", my_strerr (code_error));
    }

    data.file_name_print = "D:\\vscode\\calculator\\result.txt";

    data.fp_print = fopen (data.file_name_print, "w");

    if (data.fp_print == NULL)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FOPEN));
    }

    if ((code_error = calc_func (&stack, &data)) != ERR_NO)
    {
        fprintf (stderr, "%s", my_strerr (code_error));
    }

    if (fclose (data.fp_print) != 0)
    {
        fprintf (stderr, "%s", my_strerr (ERR_FCLOSE));
    }

    stack_dtor (&stack);

    return 0;
}
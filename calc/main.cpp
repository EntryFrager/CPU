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

    if ((code_error = calc_func (&stack, &data)) != ERR_NO)
    {
        fprintf (stderr, "%s", my_strerr (code_error));
    }

    stack_dtor (&stack);

    text_free (&data);

    return 0;
}
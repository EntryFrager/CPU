/// @file func.cpp

#include "func.h"

/**
 * Function to initialize the spu structure.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int spu_ctor (SPU *spu)
{
    my_assert (spu != NULL);

    stack_ctor (&spu->stack, STACK_DEFAULT_SIZE);
    stack_ctor (&spu->stack_call, LABEL_CNT);

    spu->file_name_input = (const char *) "..\\ass_output.bin";
    spu->file_name_print = "..\\result.txt";

    spu->fp_input = fopen (spu->file_name_input, "r + b");
    spu->fp_print = fopen (spu->file_name_print, "wb");

    if (spu->fp_print == NULL || spu->fp_input == NULL)
    {
        return ERR_FOPEN;
    }

    spu->ram_value = (ELEMENT *) calloc (SIZE_RAM, sizeof (ELEMENT));
    my_assert (spu->ram_value != NULL);

    spu->reg_value = (ELEMENT *) calloc (REG_CNT , sizeof (ELEMENT));
    my_assert (spu->reg_value != NULL);

    return ERR_NO;
}

/**
 * Function that reads information from a file.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int input_text (SPU* spu)
{
    my_assert (spu != NULL);

    spu->size_file = get_file_size (spu->fp_input) / 4;

    spu->buf = (int *) calloc (spu->size_file * sizeof (int), sizeof (char));
    my_assert (spu->buf != NULL);

    size_t read_size = fread (spu->buf, sizeof (int), spu->size_file, spu->fp_input);

    if (read_size != spu->size_file)
    {
        return ERR_FREAD;
    }

    return ERR_NO;
}

/**
 * Function returning file size.
 * @param[in] stream Pointer to file
 * @param[out] size_file File size
*/

size_t get_file_size (FILE *stream)
{
    my_assert (stream != NULL);

    size_t start = ftell (stream);
    fseek (stream, start, SEEK_END);
    size_t size_file = ftell (stream);
    rewind (stream);

    return size_file;
}

/**
 * Function that clears all variables.
 * @param[in] spu Structure containing all information
 * @param[out] code_error Returns the error code
*/

int spu_dtor(SPU *spu)
{
    my_assert (spu != NULL)

    if (fclose (spu->fp_print) != 0 || fclose (spu->fp_input) != 0)
    {
        return ERR_FCLOSE;
    }

    stack_dtor (&spu->stack);
    stack_dtor (&spu->stack_call);

    free (spu->buf);
    free (spu->reg_value);
    free (spu->ram_value);

    spu->buf       = NULL;
    spu->reg_value = NULL;
    spu->ram_value = NULL;

    spu->fp_input  = NULL;
    spu->fp_print  = NULL;

    spu->size_file = VALUE_DEFAULT;

    return ERR_NO;
}
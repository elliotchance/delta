/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @page variables-println.html
 * @brief Write one or more arguments to the stdout with a new line after each argument.
 */
DELTA_FUNCTION(println)
{
	int i;
	for(i = 0; i < DELTA_ARGS; ++i) {
		delta_vm_print_variable(DELTA_ARG(i));
		printf("\n");
	}
	
	DELTA_RETURN_NULL;
}

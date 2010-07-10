/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @brief Write one or more arguments to the stdout.
 */
DELTA_FUNCTION(print)
{
	int i;
	for(i = 0; i < DELTA_ARGS; ++i)
		delta_vm_print_variable(DELTA_ARG(i));
	
	DELTA_RETURN_NULL;
}

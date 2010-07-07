/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @brief Write one or more arguments to the stdout.
 * This does the same thing as print()
 */
DELTA_FUNCTION(echo)
{
	int i;
	for(i = 0; i < DELTA_ARGS; ++i)
		delta_vm_print_variable(ram[DELTA_ARG(i)]);
	
	DELTA_RETURN_NULL;
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @category modules/core/variables
 * 
 * @brief Write one or more arguments to the stdout with a new line after each argument.
 */
DELTA_FUNCTION(println)
{
	int i;
	for(i = 0; i < DELTA_ARGS; ++i) {
		delta_vm_print_variable(DELTA_ARG(i));
		printf("\n");
	}
	
	printf("null to %d (%d)\n", d->arg[0], d->varg[0]);
	DELTA_RETURN_NULL;
}

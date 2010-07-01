/*
 *  Delta programming language
 */

#include "ins.h"


/**
 * @brief Write one or more arguments to the stdout with a new line after each argument.
 */
delta_function(println)
{
	int i;
	for(i = 0; i < DELTA_ARGS; ++i) {
		delta_vm_print_variable(ram[DELTA_ARG(i)]);
		printf("\n");
	}
	
	DELTA_RETURN_NULL;
}

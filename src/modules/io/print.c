/*
 *  Delta programming language
 */

#include "ins.h"


/**
 * @brief Write one or more arguments to the stdout.
 */
delta_function(print)
{
	int i;
	for(i = 0; i < DELTA_ARGS; ++i)
		delta_vm_print_variable(ram[DELTA_ARG(i)]);
	
	DELTA_RETURN_NULL;
}

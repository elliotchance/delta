/*
 *  Delta programming language
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#include "ins.h"


/**
 * @brief Write one or more arguments to the stdout with a new line after each argument.
 */
ins(OUL)
{
	DELTA_PRINT_INS_ARGS(OUL);
	
	for(i = 0; i < DELTA_ARGS; ++i) {
		delta_vm_print_variable(ram[DELTA_ARG(i)]);
		printf("\n");
	}
	
	DELTA_RETURN_NULL;
}

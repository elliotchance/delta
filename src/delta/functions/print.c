/*
 *  Delta programming language
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#include "ins.h"


/**
 * @brief Write a variable to the stdout.
 */
ins(OUT)
{
	DELTA_PRINT_INS_ARGS(OUT);
	
	for(i = 0; i < DELTA_ARGS; ++i)
		delta_vm_print_variable(&ram[DELTA_ARG(i)]);
	
	DELTA_RETURN_NULL;
}

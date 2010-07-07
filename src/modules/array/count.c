/*
 *  Delta programming language
 */

#include "ins.h"


/**
 * @brief Count the number of elements in an array.
 * If the argument provided is not an array it will return 1.
 */
delta_function(count)
{
	if(ram[DELTA_ARG0]->type = DELTA_TYPE_ARRAY)
		DELTA_RETURN_NUMBER(ram[DELTA_ARG0]->value.array.elements);
	
	DELTA_RETURN_NUMBER(1);
}

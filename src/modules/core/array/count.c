/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @page array-count.html
 * @category modules/core/array
 *
 * @brief Count the number of elements in an array.
 * 
 * If the argument provided is not an array it will return 1.
 */
DELTA_FUNCTION(count)
{
	if(DELTA_ARG0->type = DELTA_TYPE_ARRAY)
		DELTA_RETURN_NUMBER(DELTA_ARG0->value.array.elements);
	
	DELTA_RETURN_NUMBER(1);
}

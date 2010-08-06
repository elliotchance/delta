/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <string.h>


/**
 * @page array-array_push.html
 * @brief Push an element onto an array.
 * 
 * If the type of variable is not an array its value will be taken and converted into the first
 * element of the array resulting in a two element array.
 */
DELTA_FUNCTION(array_push)
{
	// TODO: delta_cast_array(): first make sure source1 is an array
	if(DELTA_ARG0->type != DELTA_TYPE_ARRAY) {
		DELTA_ARG0->type = DELTA_TYPE_ARRAY;
		DELTA_ARG0->value.array.elements = 0;
		DELTA_ARG0->value.array.head = NULL;
		DELTA_ARG0->value.array.tail = NULL;
	}
	
	char *key = delta_cast_new_string(DELTA_ARG1, NULL);
	delta_array_push_kv(&DELTA_ARG0->value.array, key, DELTA_ARG2);
	
	DELTA_RETURN_ARRAY(DELTA_ARG0->value.array);
}

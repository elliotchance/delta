/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <string.h>


/**
 * @category modules/core/array
 *
 * @brief Create an array.
 */
DELTA_FUNCTION(array)
{
	struct DeltaArray array = delta_new_array();
	
	// push the elements
	int i;
	for(i = 0; i < DELTA_ARGS; ++i)
		delta_array_push_kv(&array, DELTA_ARG_NAME(i), DELTA_ARG(i));
	
	DELTA_RETURN_ARRAY(array);
}

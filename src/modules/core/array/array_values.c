/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @category modules/core/array
 *
 * @brief Return the values of an array as a new array.
 */
DELTA_FUNCTION(array_values)
{
	struct DeltaArray r = delta_new_array();
	
	int i, release;
	struct DeltaVariable *array = delta_cast_array(DELTA_ARG0, &release);
	struct DeltaArrayValue *element;
	
	DELTA_ENUMERATE_ARRAY(&array->value.array, element, i)
		delta_array_push_kv(&r, delta_int_to_string(i), element->value);
	
	DELTA_RELEASE(release, array);
	DELTA_RETURN_ARRAY(r);
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"


DELTA_FUNCTION(array_keys)
{
	struct DeltaArray r = delta_new_array();
	
	int i, release;
	struct DeltaVariable *array = delta_cast_array(DELTA_ARG0, &release);
	struct DeltaArrayValue *element;
	
	DELTA_ENUMERATE_ARRAY(&array->value.array, element, i)
		delta_array_push_k_string(&r, delta_int_to_string(i), element->key);
	
	DELTA_RELEASE(release, array);
	DELTA_RETURN_ARRAY(r);
}

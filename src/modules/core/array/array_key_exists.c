/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @category modules/core/array
 *
 * @brief Returns boolean value of if a key exists in the given array.
 */
DELTA_FUNCTION(array_key_exists)
{
	int i, release;
	struct DeltaVariable *array = delta_cast_array(DELTA_ARG0, &release);
	struct DeltaArrayValue *element;
	char *key = delta_cast_new_string(DELTA_ARG1, NULL);
	
	DELTA_ENUMERATE_ARRAY(&array->value.array, element, i) {
		if(!strcmp(element->key, key))
			DELTA_RETURN_TRUE;
	}
	
	free(key);
	DELTA_RELEASE(release, array);
	DELTA_RETURN_FALSE;
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @category modules/core/array
 *
 * @brief Returns boolean value of if a value exists in the given array.
 */
DELTA_FUNCTION(array_value_exists)
{
	int i, release;
	struct DeltaVariable *array = delta_cast_array(DELTA_ARG0, &release);
	struct DeltaArrayValue *element;
	char *cmp = delta_cast_new_string(DELTA_ARG1, NULL);
	
	DELTA_ENUMERATE_ARRAY(&array->value.array, element, i) {
		char *value = delta_cast_new_string(element->value, NULL);
		if(!strcmp(value, cmp)) {
			free(value);
			DELTA_RETURN_TRUE;
		}
		free(value);
	}
	
	free(cmp);
	DELTA_RELEASE(release, array);
	DELTA_RETURN_FALSE;
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <string.h>


/**
 * @category modules/core/array
 *
 * @brief Split a string into an array.
 */
DELTA_FUNCTION(split)
{
	char *token = delta_cast_new_string(DELTA_ARG0, NULL);
	char *str = delta_cast_new_string(DELTA_ARG1, NULL);
	struct DeltaArray r = delta_new_array();
	char *array_key;
	
	int pos = 0, i = 0, last_pos = 0;
	while((pos = delta_strpos(str + last_pos, token)) >= 0) {
		array_key = (char*) malloc(6);
		sprintf(array_key, "%d", i++);
		delta_array_push_k_string(&r, array_key, delta_copy_substring(str + last_pos, 0, pos));
		last_pos += pos + strlen(token);
	}
	
	// push the final token
	array_key = (char*) malloc(6);
	sprintf(array_key, "%d", i);
	delta_array_push_k_string(&r, array_key, delta_copy_string(str + last_pos));
	
	free(token);
	free(str);
	DELTA_RETURN_ARRAY(r);
}

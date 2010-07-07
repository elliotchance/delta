/*
 *  Delta programming language
 */

#include "ins.h"


/**
 * @brief Get the length of a string.
 */
delta_function(strlen)
{
	int release;
	struct DeltaVariable *v = delta_cast_string(DELTA_ARG0, &release);
	int r = v->size;
	
	if(release)
		free(v);
	DELTA_RETURN_NUMBER(r);
}
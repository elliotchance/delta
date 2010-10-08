/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @category modules/core/string
 * 
 * @brief Get the length of a string.
 */
DELTA_FUNCTION(strlen)
{
	if(DELTA_ARG0->type == DELTA_TYPE_STRING)
		DELTA_RETURN_NUMBER(DELTA_ARG0->size);
	
	int release;
	struct DeltaVariable *v = delta_cast_string(DELTA_ARG0, &release);
	int r = v->size;
	
	DELTA_RELEASE(release, v);
	DELTA_RETURN_NUMBER(r);
}

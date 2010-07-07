/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @brief Get the length of a string.
 */
DELTA_FUNCTION(strlen)
{
	int release;
	struct DeltaVariable *v = delta_cast_string(DELTA_ARG0, &release);
	int r = v->size;
	
	if(release)
		free(v);
	DELTA_RETURN_NUMBER(r);
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @page string-strlen.html
 * @brief Get the length of a string.
 */
DELTA_FUNCTION(strlen)
{
	int release;
	struct DeltaVariable *v = delta_cast_string(DELTA_ARG0, &release);
	int r = v->size;
	
	DELTA_RELEASE(release, v);
	DELTA_RETURN_NUMBER(r);
}

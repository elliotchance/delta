/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <string.h>


/**
 * @page string-substr.html
 * @brief Fetch a substring.
 */
DELTA_FUNCTION(substr)
{
	// prepare incoming arguments
	int release_arg0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	int arg1 = (int) delta_cast_number(DELTA_ARG1);
	int arg2;
	
	// 2 argument substr() means all characters after the start position
	if(DELTA_ARGS < 3)
		arg2 = arg0->size - arg1;
	else
		arg2 = (int) delta_cast_number(DELTA_ARG2);
	
	// be nice and release
	DELTA_RELEASE(release_arg0, arg0);
	
	// return
	DELTA_DEST->type = DELTA_TYPE_STRING;
	DELTA_DEST->value.ptr = (char*) malloc(arg2 + 1);
	strncpy(DELTA_DEST->value.ptr, arg0->value.ptr + arg1, arg2);
}

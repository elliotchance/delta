/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <string.h>
#include <ctype.h>


/**
 * @category modules/core/string
 * 
 * @brief Trim left side of a string of whitespace.
 */
DELTA_FUNCTION(ltrim)
{
	// prepare incoming arguments
	int release_arg0, i;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	
	// count the spaces
	int spaces = 0;
	for(i = 0; i < arg0->size; ++i) {
		if(!isspace(arg0->value.ptr[i]))
		   break;
		++spaces;
	}
	
	// be nice and release
	DELTA_RELEASE(release_arg0, arg0);
	
	// return
	DELTA_RETURN_BINARY_STRING(arg0->value.ptr + spaces, arg0->size - spaces);
}

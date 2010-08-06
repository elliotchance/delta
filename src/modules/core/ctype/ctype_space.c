/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <ctype.h>


/**
 * @category modules/core/ctype
 *
 * @brief Check for whitespace character(s).
 * @syntax bool ctype_space ( string text )
 *
 * Checks if all of the characters in the provided string , text, creates whitespace.
 *
 * @param text The tested string.
 * @return Returns TRUE if every character in text creates some sort of white space, FALSE
 *         otherwise. Besides the blank character this also includes tab, vertical tab, line feed,
 *         carriage return and form feed characters.
 */
DELTA_FUNCTION(ctype_space)
{
	int release0, r = DELTA_TRUE, i;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	
	// process
	for(i = 0; i < arg0->size; ++i) {
		if(!isspace(arg0->value.ptr[i])) {
			r = DELTA_FALSE;
			break;
		}
	}
	
	// clean up
	DELTA_RELEASE(release0, arg0);
	DELTA_RETURN_BOOLEAN(r);
}

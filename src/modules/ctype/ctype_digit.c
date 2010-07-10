/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <ctype.h>


/**
 * @brief Check for numeric character(s).
 * @syntax bool ctype_digit ( string text )
 *
 * Checks if all of the characters in the provided string, text, are numerical.
 *
 * @param text The tested string.
 * @return Returns TRUE if every character in the string text is a decimal digit, FALSE otherwise.
 */
DELTA_FUNCTION(ctype_digit)
{
	int release0, r = DELTA_TRUE, i;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	
	// process
	for(i = 0; i < arg0->size; ++i) {
		if(!isdigit(arg0->value.ptr[i])) {
			r = DELTA_FALSE;
			break;
		}
	}
	
	// clean up
	DELTA_RELEASE(release0, arg0);
	DELTA_RETURN_BOOLEAN(r);
}

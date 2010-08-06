/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <ctype.h>


/**
 * @category modules/core/ctype
 *
 * @brief Check for lowercase character(s).
 * @syntax bool ctype_lower ( string text )
 *
 * Checks if all of the characters in the provided string , text, are lowercase letters.
 *
 * @param text The tested string.
 * @return Returns TRUE if every character in text is a lowercase letter in the current locale.
 */
DELTA_FUNCTION(ctype_lower)
{
	int release0, r = DELTA_TRUE, i;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	
	// process
	for(i = 0; i < arg0->size; ++i) {
		if(!islower(arg0->value.ptr[i])) {
			r = DELTA_FALSE;
			break;
		}
	}
	
	// clean up
	DELTA_RELEASE(release0, arg0);
	DELTA_RETURN_BOOLEAN(r);
}

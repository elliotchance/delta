/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <ctype.h>


/**
 * @page ctype-alnum.html
 * @category modules/core/ctype
 *
 * @brief Check for alphanumeric character(s).
 * @syntax bool ctype_alnum ( string text )
 *
 * Checks if all of the characters in the provided string , text, are alphanumeric. In the standard
 * C locale letters are just [A-Za-z].
 *
 * @param text The tested string.
 * @return Returns TRUE if every character in text is either a letter or a digit, FALSE otherwise.
 */
DELTA_FUNCTION(ctype_alnum)
{
	int release0, r = DELTA_TRUE, i;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	
	// process
	for(i = 0; i < arg0->size; ++i) {
		if(!isalnum(arg0->value.ptr[i])) {
			r = DELTA_FALSE;
			break;
		}
	}
	
	// clean up
	DELTA_RELEASE(release0, arg0);
	DELTA_RETURN_BOOLEAN(r);
}

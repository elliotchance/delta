/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <ctype.h>


/**
 * @category modules/core/ctype
 *
 * @brief Check for character(s) representing a hexadecimal digit.
 * @syntax bool ctype_xdigit ( string text )
 *
 * Checks if all of the characters in the provided string , text, are hexadecimal 'digits'.
 *
 * @param text The tested string.
 * @return Returns TRUE if every character in text is a hexadecimal 'digit', that is a decimal digit
 *         or a character from [A-Fa-f] , FALSE otherwise.
 */
DELTA_FUNCTION(ctype_xdigit)
{
	int release0, r = DELTA_TRUE, i;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	
	// process
	for(i = 0; i < arg0->size; ++i) {
		if(!isxdigit(arg0->value.ptr[i])) {
			r = DELTA_FALSE;
			break;
		}
	}
	
	// clean up
	DELTA_RELEASE(release0, arg0);
	DELTA_RETURN_BOOLEAN(r);
}

/*
 *  Delta programming language
 */

#include "ins.h"
#include <ctype.h>


/**
 * @brief Check for character(s) representing a hexadecimal digit.
 * @syntax bool ctype_xdigit ( string text )
 *
 * Checks if all of the characters in the provided string , text, are hexadecimal 'digits'.
 *
 * @param text The tested string.
 * @return Returns TRUE if every character in text is a hexadecimal 'digit', that is a decimal digit
 *         or a character from [A-Fa-f] , FALSE otherwise.
 */
delta_function(ctype_xdigit)
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
	if(release0)
		free(arg0);
	
	DELTA_RETURN_BOOLEAN(r);
}

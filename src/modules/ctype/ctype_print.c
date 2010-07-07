/*
 *  Delta programming language
 */

#include "ins.h"
#include <ctype.h>


/**
 * @brief Check for printable character(s).
 * @syntax bool ctype_print ( string text )
 *
 * Checks if all of the characters in the provided string, text, are printable.
 *
 * @param text The tested string.
 * @return Returns TRUE if every character in text will actually create output (including blanks).
 * Returns FALSE if text contains control characters or characters that do not have any output or
 * control function at all.
 */
delta_function(ctype_print)
{
	int release0, r = DELTA_TRUE, i;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	
	// process
	for(i = 0; i < arg0->size; ++i) {
		if(!isprint(arg0->value.ptr[i])) {
			r = DELTA_FALSE;
			break;
		}
	}
	
	// clean up
	if(release0)
		free(arg0);
	
	DELTA_RETURN_BOOLEAN(r);
}
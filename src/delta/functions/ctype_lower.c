/*
 *  Delta programming language
 */

#include "ins.h"
#include <ctype.h>


/**
 * @brief Check for lowercase character(s).
 * @syntax bool ctype_lower ( string text )
 *
 * Checks if all of the characters in the provided string , text, are lowercase letters.
 *
 * @param text The tested string.
 * @return Returns TRUE if every character in text is a lowercase letter in the current locale.
 */
delta_function(ctype_lower)
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
	if(release0)
		free(arg0);
	
	DELTA_RETURN_BOOLEAN(r);
}

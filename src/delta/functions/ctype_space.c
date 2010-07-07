/*
 *  Delta programming language
 */

#include "ins.h"
#include <ctype.h>


/**
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
delta_function(ctype_space)
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
	if(release0)
		free(arg0);
	
	DELTA_RETURN_BOOLEAN(r);
}

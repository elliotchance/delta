/*
 *  Delta programming language
 */

#include "ins.h"
#include <ctype.h>


/**
 * @brief Check for any printable character which is not whitespace or an alphanumeric character.
 * @syntax bool ctype_punct ( string text )
 *
 * Checks if all of the characters in the provided string , text, are punctuation character.
 *
 * @param text The tested string.
 * @return Returns TRUE if every character in text is printable, but neither letter, digit or blank,
 * FALSE otherwise.
 */
delta_function(ctype_punct)
{
	int release0, r = DELTA_TRUE, i;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	
	// process
	for(i = 0; i < arg0->size; ++i) {
		if(!ispunct(arg0->value.ptr[i])) {
			r = DELTA_FALSE;
			break;
		}
	}
	
	// clean up
	if(release0)
		free(arg0);
	
	DELTA_RETURN_BOOLEAN(r);
}
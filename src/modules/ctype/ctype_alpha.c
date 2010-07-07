/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <ctype.h>


/**
 * @brief Check for alphabetic character(s).
 * @syntax bool ctype_alpha ( string text )
 *
 * Checks if all of the characters in the provided string , text, are alphabetic. In the standard C
 * locale letters are just [A-Za-z] and ctype_alpha() is equivalent to (ctype_upper(text) ||
 * ctype_lower(text)) if text is just a single character, but other languages have letters that are
 * considered neither upper nor lower case.
 *
 * @param text The tested string.
 * @return Returns TRUE if every character in text is a letter from the current locale, FALSE
 *         otherwise.
 */
DELTA_FUNCTION(ctype_alpha)
{
	int release0, r = DELTA_TRUE, i;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	
	// process
	for(i = 0; i < arg0->size; ++i) {
		if(!isalpha(arg0->value.ptr[i])) {
			r = DELTA_FALSE;
			break;
		}
	}
	
	// clean up
	if(release0)
		free(arg0);
	
	DELTA_RETURN_BOOLEAN(r);
}

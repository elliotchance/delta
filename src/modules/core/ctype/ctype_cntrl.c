/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <ctype.h>


/**
 * @page ctype-cntrl.html
 * @category modules/core/ctype
 *
 * @brief Check for control character(s).
 * @syntax bool ctype_cntrl ( string text )
 *
 * Checks if all of the characters in the provided string , text, are control characters. Control
 * characters are e.g. line feed, tab, escape.
 *
 * @param text The tested string.
 * @return Returns TRUE if every character in text is a control character from the current locale,
 *         FALSE otherwise.
 */
DELTA_FUNCTION(ctype_cntrl)
{
	int release0, r = DELTA_TRUE, i;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	
	// process
	for(i = 0; i < arg0->size; ++i) {
		if(!iscntrl(arg0->value.ptr[i])) {
			r = DELTA_FALSE;
			break;
		}
	}
	
	// clean up
	DELTA_RELEASE(release0, arg0);
	DELTA_RETURN_BOOLEAN(r);
}

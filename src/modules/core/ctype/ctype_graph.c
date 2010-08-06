/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <ctype.h>


/**
 * @category modules/core/ctype
 *
 * @brief Check for any printable character(s) except space.
 * @syntax bool ctype_graph ( string text )
 *
 * Checks if all of the characters in the provided string, text, creates visible output.
 *
 * @param text The tested string.
 * @return Returns TRUE if every character in text is printable and actually creates visible output
 * (no white space), FALSE otherwise.
 */
DELTA_FUNCTION(ctype_graph)
{
	int release0, r = DELTA_TRUE, i;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	
	// process
	for(i = 0; i < arg0->size; ++i) {
		if(!isgraph(arg0->value.ptr[i])) {
			r = DELTA_FALSE;
			break;
		}
	}
	
	// clean up
	DELTA_RELEASE(release0, arg0);
	DELTA_RETURN_BOOLEAN(r);
}

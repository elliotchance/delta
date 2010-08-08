/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <string.h>
#include <ctype.h>


/**
 * @category modules/core/string
 * 
 * @brief Trim right side of a string of whitespace.
 */
DELTA_FUNCTION(rtrim)
{
	// prepare incoming arguments
	int size, i;
	char *arg0 = delta_cast_new_string(DELTA_ARG0, &size);
	
	// count the spaces
	int spaces = 0;
	for(i = size - 1; i >= 0; --i) {
		if(!isspace(arg0[i]))
		   break;
		++spaces;
	}
	arg0[size - spaces] = '\0';
	
	// return
	DELTA_RETURN_BINARY_STRING(arg0, size - spaces);
}

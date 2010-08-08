/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <string.h>
#include <ctype.h>


/**
 * @category modules/core/string
 * 
 * @brief Trim left and right side of a string of whitespace.
 */
DELTA_FUNCTION(trim)
{
	// prepare incoming arguments
	int size, i;
	char *arg0 = delta_cast_new_string(DELTA_ARG0, &size);
	
	// count the ending spaces
	int end_spaces = 0;
	for(i = size - 1; i >= 0; --i) {
		if(!isspace(arg0[i]))
			break;
		++end_spaces;
	}
	arg0[size - end_spaces] = '\0';
	
	// count the begining spaces
	int begin_spaces = 0;
	for(i = 0; i < size; ++i) {
		if(!isspace(arg0[i]))
			break;
		++begin_spaces;
	}
	
	// return
	DELTA_RETURN_BINARY_STRING(arg0 + begin_spaces, size - end_spaces);
}

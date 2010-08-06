/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <string.h>
#include <ctype.h>


/**
 * @category modules/core/string
 * 
 * @brief Make a string uppercase.
 * @syntax string strtoupper ( string string )
 *
 * Returns \p string with all alphabetic characters converted to uppercase.
 *
 * @example Example #1 strtoupper() example
 * str = "Mary Had A Little Lamb and She LOVED It So";
 * str = strtoupper(str);
 * println(str); // Prints MARY HAD A LITTLE LAMB AND SHE LOVED IT SO
 * @endexample
 *
 * @return Returns the uppercased string.
 */
DELTA_FUNCTION(strtoupper)
{
	// prepare incoming arguments
	int release_arg0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	
	// convert each character to uppercase
	int i;
	for(i = 0; i < arg0->size; ++i)
		arg0->value.ptr[i] = toupper(arg0->value.ptr[i]);
	
	// be nice and release
	DELTA_RELEASE(release_arg0, arg0);
	DELTA_RETURN_BINARY_STRING(arg0->value.ptr, arg0->size);
}

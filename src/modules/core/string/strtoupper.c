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
	int size;
	char *arg0 = delta_cast_new_string(DELTA_ARG0, &size);
	
	// convert each character to uppercase
	int i;
	for(i = 0; i < size; ++i)
		arg0[i] = toupper(arg0[i]);
	
	DELTA_RETURN_BINARY_STRING(arg0, size);
}

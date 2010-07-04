/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>
#include <string.h>


// from base_convert.c
char* base_convert(char* in, int base1, int base2);


/**
 * @brief Decimal to hexadecimal.
 * @syntax string dechex ( int number )
 *
 * Returns a string containing a hexadecimal representation of the given number argument. The
 * largest number that can be converted is 4294967295 in decimal resulting to "ffffffff".
 *
 * @param number Decimal value to convert.
 * @return Hexadecimal string representation of number.
 * @see bindec
 * @see decoct
 * @see decbin
 * @see base_convert
 */
delta_function(dechex)
{
	int release_arg0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	char* r = base_convert(arg0->value.ptr, 10, 16);
	
	// clean up
	if(release_arg0)
		free(arg0);
	
	DELTA_RETURN_STRING(r);
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>
#include <string.h>


// from base_convert.c
char* base_convert(char* in, int base1, int base2);


/**
 * @brief Decimal to binary.
 * @syntax string decbin ( int number )
 *
 * Returns a string containing a binary representation of the given number argument.
 *
 * @param number Decimal value to convert.
 * @return Binary string representation of number.
 * @see bindec
 * @see decoct
 * @see dechex
 * @see base_convert
 */
DELTA_FUNCTION(decbin)
{
	int release_arg0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	char* r = base_convert(arg0->value.ptr, 10, 2);
	
	// clean up
	DELTA_RELEASE(release_arg0, arg0);
	DELTA_RETURN_STRING(r);
}

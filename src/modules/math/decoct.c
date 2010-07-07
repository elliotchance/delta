/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>
#include <string.h>


// from base_convert.c
char* base_convert(char* in, int base1, int base2);


/**
 * @brief Decimal to octal.
 * @syntax string decoct ( int number )
 *
 * Returns a string containing an octal representation of the given number argument. The largest
 * number that can be converted is 4294967295 in decimal resulting to "37777777777".
 *
 * @param number Decimal value to convert.
 * @return Octal string representation of number.
 * @see bindec
 * @see dechex
 * @see decbin
 * @see base_convert
 */
DELTA_FUNCTION(decoct)
{
	int release_arg0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	char* r = base_convert(arg0->value.ptr, 10, 8);
	
	// clean up
	if(release_arg0)
		free(arg0);
	
	DELTA_RETURN_STRING(r);
}

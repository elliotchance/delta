/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>
#include <string.h>


// from base_convert.c
char* base_convert(char* in, int base1, int base2);


/**
 * @category modules/core/math
 * 
 * @brief Octal to decimal.
 * @syntax number octdec ( string octal_string )
 *
 * Returns the decimal equivalent of the octal number represented by the octal_string argument.
 *
 * @param octal_string The octal string to convert.
 * @return The decimal representation of octal_string.
 * @see bindec
 * @see dechex
 * @see decbin
 * @see base_convert
 */
DELTA_FUNCTION(octdec)
{
	int release_arg0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	char* r = base_convert(arg0->value.ptr, 8, 10);
	
	// clean up
	DELTA_RELEASE(release_arg0, arg0);
	DELTA_RETURN_STRING(r);
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>
#include <string.h>


// from base_convert.c
char* base_convert(char* in, int base1, int base2);


/**
 * @page math-hexdec.html
 * @category modules/core/math
 * 
 * @brief Hexadecimal to decimal.
 * @syntax string hexdoc ( int hex_string )
 *
 * Returns the decimal equivalent of the hexadecimal number represented by the hex_string argument.
 * hexdec() converts a hexadecimal string to a decimal number.
 *
 * @param hex_string The hexadecimal string to convert.
 * @return The decimal representation of hex_string.
 * @see bindec
 * @see dechex
 * @see decbin
 * @see base_convert
 */
DELTA_FUNCTION(hexdec)
{
	int release_arg0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	char* r = base_convert(arg0->value.ptr, 16, 10);
	
	// clean up
	DELTA_RELEASE(release_arg0, arg0);
	DELTA_RETURN_STRING(r);
}

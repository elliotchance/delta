/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>
#include <string.h>


// from base_convert.c
char* base_convert(char* in, int base1, int base2);


/**
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
delta_function(hexdec)
{
	int release_arg0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	char* r = base_convert(arg0->value.ptr, 16, 10);
	
	// clean up
	if(release_arg0)
		free(arg0);
	
	DELTA_RETURN_STRING(r);
}

/*
 *  Delta programming language
 */

#include "ins.h"
#include "xtoa.h"
#include <math.h>
#include <string.h>


// from base_convert.c
char* base_convert(char* in, int base1, int base2);


/**
 * @brief Binary to decimal.
 * @syntax number bindec ( string binary_string )
 *
 * Returns the decimal equivalent of the binary number represented by the binary_string argument.
 *
 * @param binary_string The number to convert.
 * @return The decimal value of binary_string.
 * @see decbin
 * @see octdec
 * @see hexdec
 * @see base_convert
 */
delta_function(bindec)
{
	int release_arg0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	char* r = base_convert(arg0->value.ptr, 2, 10);
	
	// clean up
	if(release_arg0)
		free(arg0);
	
	DELTA_RETURN_STRING(r);
}

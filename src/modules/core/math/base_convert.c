/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "xtoa.h"
#include <math.h>
#include <string.h>


int base_convert_chr2int(char ch)
{
	static char *chs = "0123456789abcdefABCDEF";
	int chvals[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 10, 11, 12, 13, 14, 15 };
	int i;
	for(i = 0; i < 22; ++i) {
		if(chs[i] == ch)
			return chvals[i];
	}
	return 0;
}


char* base_convert(char* in, int base1, int base2)
{
	int in_len = strlen(in);
	
	// convert the incoming base to base 10
	int r = 0, i, j;
	for(i = in_len - 1, j = 0; i >= 0; --i, ++j)
		r += base_convert_chr2int(in[i]) * (int) pow((double) base1, (double) j);
	
	// convert base 10 to outgoing base
	char *out = (char*) malloc(32);
	itoa(r, out, base2);
	
	return out;
}


/**
 * @page math-base_convert.html
 * @category modules/core/math
 * 
 * @brief Convert a number between arbitrary bases.
 * @syntax string base_convert ( string number , int frombase , int tobase )
 *
 * Returns a string containing number represented in base tobase. The base in which number is given
 * is specified in frombase. Both frombase and tobase have to be between 2 and 36, inclusive. Digits
 * in numbers with a base higher than 10 will be represented with the letters a-z, with a meaning
 * 10, b meaning 11 and z meaning 35.
 *
 * @param number The number to convert.
 * @param frombase The base number is in.
 * @param tobase The base to convert number to.
 * @return number converted to base tobase.
 */
DELTA_FUNCTION(base_convert)
{
	int release_arg0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	double base1 = delta_cast_number(DELTA_ARG1);
	double base2 = delta_cast_number(DELTA_ARG2);
	char *r = base_convert(arg0->value.ptr, base1, base2);
	
	// clean up
	DELTA_RELEASE(release_arg0, arg0);
	DELTA_RETURN_STRING(r);
}

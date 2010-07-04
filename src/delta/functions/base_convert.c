/*
 *  Delta programming language
 */

#include "ins.h"
#include "../../modules/xtoa.h"
#include <math.h>
#include <string.h>


int base_convert_chr2int(char ch)
{
	static char *chs = "0123456789abcdefABCDEF";
	int chvals[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 10, 11, 12, 13, 14, 15 };
	for(int i = 0; i < 22; ++i) {
		if(chs[i] == ch)
			return chvals[i];
	}
	return 0;
}


/**
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
delta_function(base_convert)
{
	int release_arg0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	double base1 = delta_cast_number(DELTA_ARG1);
	double base2 = delta_cast_number(DELTA_ARG2);
	
	// convert the incoming base to base 10
	int r = 0;
	for(int i = arg0->size - 1, j = 0; i >= 0; --i, ++j)
		r += base_convert_chr2int(arg0->value.ptr[i]) * (int) pow((double) base1, (double) j);
	
	// convert base 10 to outgoing base
	char *out = (char*) malloc(32);
	itoa(r, out, base2);
	
	// clean up
	if(release_arg0)
		free(arg0);
	
	DELTA_RETURN_STRING(out);
}

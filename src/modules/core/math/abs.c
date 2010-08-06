/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 *
 * @brief Absolute value.
 * @syntax number abs ( number number )
 *
 * Returns the absolute value of number.
 *
 * @param number The numeric value to process.
 * @return The absolute value of number.
 * 
 * @example Example #1 abs() example
 * abs = abs(-4.2); // $abs = 4.2; (double/float)
 * abs2 = abs(5);   // $abs2 = 5; (integer)
 * abs3 = abs(-5);  // $abs3 = 5; (integer)
 * @endexample
 *
 * @see gmp_abs
 * @see gmp_sign
 */
DELTA_FUNCTION(abs)
{
	double number = delta_cast_number(DELTA_ARG0);
	DELTA_RETURN_NUMBER((number < 0) ? -number : number);
}

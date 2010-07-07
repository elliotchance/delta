/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Square root.
 * @syntax number sqrt ( number arg )
 *
 * Returns the square root of arg.
 *
 * @param arg The argument to process.
 * @return The square root of arg or the special value NAN for negative numbers.
 *
 * @example Example #1 sqrt() example
 * // Precision depends on your precision directive
 * println(sqrt(9));  // 3
 * println(sqrt(10)); // 3.16227766 ...
 * @endexample
 *
 * @see pow
 */
delta_function(sqrt)
{
	double temp = delta_cast_number(DELTA_ARG0);
	if(temp < 0)
		DELTA_RETURN_NAN;
	
	DELTA_RETURN_NUMBER(sqrt(temp));
}

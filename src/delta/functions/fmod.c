/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Returns the floating point remainder (modulo) of the division of the arguments.
 * @syntax number fmod ( number x , number y )
 *
 * Returns the floating point remainder of dividing the dividend (x) by the divisor (y). The
 * reminder (r) is defined as: x = i * y + r, for some integer i. If y is non-zero, r has the same
 * sign as x and a magnitude less than the magnitude of y.
 *
 * @param x The dividend.
 * @param y The divisor.
 * @return The floating point remainder of x/y.
 */
delta_function(fmod)
{
	DELTA_RETURN_NUMBER(fmod(delta_cast_number(DELTA_ARG0), delta_cast_number(DELTA_ARG1)));
}

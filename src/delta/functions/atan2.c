/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Arc tangent of two variables.
 * 
 * This function calculates the arc tangent of the two variables x and y. It is similar to
 * calculating the arc tangent of y / x, except that the signs of both arguments are used to
 * determine the quadrant of the result.
 * 
 * The function returns the result in radians, which is between -PI and PI (inclusive).
 *
 * @param y Dividend parameter.
 * @param x Divisor parameter.
 * @return The arc tangent of y/x in radians.
 */
delta_function(atan2)
{
	DELTA_RETURN_NUMBER(atan2(delta_cast_number(DELTA_ARG0), delta_cast_number(DELTA_ARG1)));
}

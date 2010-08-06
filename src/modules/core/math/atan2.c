/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-atan2.html
 * @brief Arc tangent of two variables.
 * @syntax number atan2 ( number y , number x )
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
 * @see atan
 */
DELTA_FUNCTION(atan2)
{
	DELTA_RETURN_NUMBER(atan2(delta_cast_number(DELTA_ARG0), delta_cast_number(DELTA_ARG1)));
}

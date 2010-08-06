/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Sine.
 * @syntax number sin ( number arg )
 *
 * sin() returns the sine of the arg parameter. The arg parameter is in radians.
 *
 * @param arg A value in radians.
 * @return The sine of arg.
 *
 * @example Example #1 sin() example
 * // Precision depends on your precision directive
 * println(sin(deg2rad(60)));  //  0.866025403 ...
 * println(sin(60));           // -0.304810621 ...
 * @endexample
 *
 * @see asin
 * @see sinh
 * @see cos
 * @see tan
 * @see deg2rad
 */
DELTA_FUNCTION(sin)
{
	DELTA_RETURN_NUMBER(sin(delta_cast_number(DELTA_ARG0)));
}

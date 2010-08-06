/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Tangent.
 * @syntax number tan ( number arg )
 *
 * tan() returns the tangent of the arg parameter. The arg parameter is in radians.
 *
 * @param arg The argument to process in radians.
 * @return The tangent of arg.
 *
 * @example Example #1 tan() example
 * println(tan(M_PI_4)); // 1
 * @endexample
 *
 * @see atan
 * @see atan2
 * @see sin
 * @see cos
 * @see tanh
 * @see deg2rad
 */
DELTA_FUNCTION(tan)
{
	DELTA_RETURN_NUMBER(tan(delta_cast_number(DELTA_ARG0)));
}

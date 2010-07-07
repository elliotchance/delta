/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Cosine.
 * @syntax number cos ( number arg )
 *
 * cos() returns the cosine of the arg parameter. The arg parameter is in radians.
 *
 * @param arg An angle in radians.
 * @return The cosine of arg.
 *
 * @example Example #1 cos() example
 * println(cos(M_PI));
 * @endexample
 *
 * @see acos
 * @see sin
 * @see tan
 * @see deg2rad
 */
delta_function(cos)
{
	DELTA_RETURN_NUMBER(cos(delta_cast_number(DELTA_ARG0)));
}

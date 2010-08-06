/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Arc sine.
 * @syntax number asin ( number arg )
 *
 * Returns the arc sine of arg in radians. asin() is the complementary function of sin(), which
 * means that a==sin(asin(a)) for every value of a that is within asin()'s range.
 * 
 * @param arg The argument to process.
 * @return The arc sine of arg in radians.
 * @see sin
 * @see asinh
 * @see acos
 * @see atan
 */
DELTA_FUNCTION(asin)
{
	DELTA_RETURN_NUMBER(asin(delta_cast_number(DELTA_ARG0)));
}

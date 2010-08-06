/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Arc tangent.
 * @syntax number atan ( number arg )
 *
 * Returns the arc tangent of arg in radians. atan() is the complementary function of tan(), which
 * means that a==tan(atan(a)) for every value of a that is within atan()'s range.
 *
 * @param The argument to process.
 * @return The arc tangent of arg in radians.
 * @see tan
 * @see atanh
 * @see asin
 * @see acos
 */
DELTA_FUNCTION(atan)
{
	DELTA_RETURN_NUMBER(atan(delta_cast_number(DELTA_ARG0)));
}

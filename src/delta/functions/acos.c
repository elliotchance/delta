/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Arc cosine.
 * @syntax number acos ( number arg )
 *
 * Returns the arc cosine of arg in radians. acos() is the complementary function of cos(), which
 * means that a==cos(acos(a)) for every value of a that is within acos()' range.
 *
 * @param arg The argument to process
 * @return The arc cosine of arg in radians.
 * @see cos
 * @see acosh
 * @see asin
 * @see atan
 */
delta_function(acos)
{
	DELTA_RETURN_NUMBER(acos(delta_cast_number(DELTA_ARG0)));
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-asinh.html
 * @category modules/core/math
 * 
 * @brief Inverse hyperbolic sine.
 * @syntax number asinh ( number arg )
 *
 * Returns the inverse hyperbolic sine of arg, i.e. the value whose hyperbolic sine is arg.
 *
 * @param arg The argument to process.
 * @return The inverse hyperbolic sine of arg.
 * @see sinh
 * @see asin
 * @see acosh
 * @see atanh
 */
DELTA_FUNCTION(asinh)
{
	DELTA_RETURN_NUMBER(asinh(delta_cast_number(DELTA_ARG0)));
}

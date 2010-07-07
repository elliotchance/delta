/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @brief Inverse hyperbolic cosine.
 * @syntax number acosh ( number arg )
 *
 * Returns the inverse hyperbolic cosine of arg, i.e. the value whose hyperbolic cosine is arg.
 * 
 * @param arg The value to process.
 * @return The inverse hyperbolic cosine of arg.
 * @see cosh
 * @see acos
 * @see asinh
 * @see atanh
 */
DELTA_FUNCTION(acosh)
{
	DELTA_RETURN_NUMBER(acosh(delta_cast_number(DELTA_ARG0)));
}

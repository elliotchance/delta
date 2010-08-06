/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-atanh.html
 * @brief Inverse hyperbolic tangent.
 * @syntax number atanh ( number arg )
 *
 * Returns the inverse hyperbolic tangent of arg, i.e. the value whose hyperbolic tangent is arg.
 *
 * @param arg The argument to process.
 * @return Inverse hyperbolic tangent of arg.
 * @see tanh
 * @see atan
 * @see asinh
 * @see acosh
 */
DELTA_FUNCTION(atanh)
{
	DELTA_RETURN_NUMBER(atanh(delta_cast_number(DELTA_ARG0)));
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-tanh.html
 * @brief Hyperbolic tangent.
 * @syntax number tanh ( number arg )
 *
 * Returns the hyperbolic tangent of arg, defined as sinh(arg)/cosh(arg).
 *
 * @param arg The argument to process.
 * @return The hyperbolic tangent of arg.
 *
 * @see tan
 * @see atanh
 * @see sinh
 * @see cosh
 */
DELTA_FUNCTION(tanh)
{
	DELTA_RETURN_NUMBER(tanh(delta_cast_number(DELTA_ARG0)));
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Hyperbolic sine.
 * @syntax number sinh ( number arg )
 *
 * Returns the hyperbolic sine of arg, defined as (exp(arg) - exp(-arg))/2.
 *
 * @param arg The argument to process.
 * @return The hyperbolic sine of arg.
 *
 * @see sin
 * @see asinh
 * @see cosh
 * @see tanh
 */
DELTA_FUNCTION(sinh)
{
	DELTA_RETURN_NUMBER(sinh(delta_cast_number(DELTA_ARG0)));
}

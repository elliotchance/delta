/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
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
delta_function(asinh)
{
	DELTA_RETURN_NUMBER(asinh(delta_cast_number(DELTA_ARG0)));
}
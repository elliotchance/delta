/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
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
delta_function(atanh)
{
	DELTA_RETURN_NUMBER(atanh(delta_cast_number(DELTA_ARG0)));
}

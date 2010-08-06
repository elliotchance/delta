/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-expm1.html
 * @brief Returns exp(number) - 1, computed in a way that is accurate even when the value of number
 *        is close to zero.
 * @syntax number expm1 ( number arg )
 *
 * expm1() returns the equivalent to 'exp(arg) - 1' computed in a way that is accurate even if the
 * value of arg is near zero, a case where 'exp (arg) - 1' would be inaccurate due to subtraction of
 * two numbers that are nearly equal.
 *
 * @param arg The argument to process.
 * @return 'e' to the power of arg minus one.
 * @see log1p
 * @see exp
 */
DELTA_FUNCTION(expm1)
{
	DELTA_RETURN_NUMBER(expm1(delta_cast_number(DELTA_ARG0)));
}

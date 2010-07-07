/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Hyperbolic cosine.
 * @syntax number cosh ( number arg )
 *
 * Returns the hyperbolic cosine of arg, defined as (exp(arg) + exp(-arg))/2.
 *
 * @param arg The argument to process.
 * @return The hyperbolic cosine of arg.
 *
 * @see cos
 * @see acosh
 * @see sinh
 */
delta_function(cosh)
{
	DELTA_RETURN_NUMBER(cosh(delta_cast_number(DELTA_ARG0)));
}

/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Arc hyperbolic sine.
 */
delta_function(asinh)
{
	DELTA_RETURN_NUMBER(asinh(delta_cast_number(DELTA_ARG0)));
}

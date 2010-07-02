/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Arc hyperbolic tangent.
 */
delta_function(atanh)
{
	DELTA_RETURN_NUMBER(atanh(delta_cast_number(DELTA_ARG0)));
}

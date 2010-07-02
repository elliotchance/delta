/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Arc hyperbolic cosine.
 */
delta_function(acosh)
{
	DELTA_RETURN_NUMBER(acosh(delta_cast_number(DELTA_ARG0)));
}

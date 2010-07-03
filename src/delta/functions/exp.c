/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Calculates the exponent of e.
 */
delta_function(exp)
{
	DELTA_RETURN_NUMBER(exp(delta_cast_number(DELTA_ARG0)));
}

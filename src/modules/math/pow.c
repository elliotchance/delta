/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Exponential expression.
 */
delta_function(pow)
{
	DELTA_RETURN_NUMBER(pow(delta_cast_number(DELTA_ARG0), delta_cast_number(DELTA_ARG1)));
}

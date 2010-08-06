/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Exponential expression.
 */
DELTA_FUNCTION(pow)
{
	DELTA_RETURN_NUMBER(pow(delta_cast_number(DELTA_ARG0), delta_cast_number(DELTA_ARG1)));
}

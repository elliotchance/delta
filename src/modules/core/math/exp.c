/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Calculates the exponent of e.
 */
DELTA_FUNCTION(exp)
{
	DELTA_RETURN_NUMBER(exp(delta_cast_number(DELTA_ARG0)));
}

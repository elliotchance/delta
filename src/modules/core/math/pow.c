/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-pow.html
 * @brief Exponential expression.
 */
DELTA_FUNCTION(pow)
{
	DELTA_RETURN_NUMBER(pow(delta_cast_number(DELTA_ARG0), delta_cast_number(DELTA_ARG1)));
}

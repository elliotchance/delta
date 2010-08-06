/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Natural logarithm.
 */
DELTA_FUNCTION(log)
{
	DELTA_RETURN_NUMBER(log(delta_cast_number(DELTA_ARG0)));
}

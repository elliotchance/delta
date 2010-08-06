/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-floor.html
 * @category modules/core/math
 * 
 * @brief Round down to the nearest integer.
 */
DELTA_FUNCTION(floor)
{
	DELTA_RETURN_NUMBER(floor(delta_cast_number(DELTA_ARG0)));
}

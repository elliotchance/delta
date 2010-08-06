/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-log10.html
 * @category modules/core/math
 * 
 * @brief Base-10 logarithm.
 */
DELTA_FUNCTION(log10)
{
	DELTA_RETURN_NUMBER(log10(delta_cast_number(DELTA_ARG0)));
}

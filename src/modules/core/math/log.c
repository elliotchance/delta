/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-log.html
 * @brief Natural logarithm.
 */
DELTA_FUNCTION(log)
{
	DELTA_RETURN_NUMBER(log(delta_cast_number(DELTA_ARG0)));
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-ceil.html
 * @brief Round up to the nearest integer.
 */
DELTA_FUNCTION(ceil)
{
	DELTA_RETURN_NUMBER(ceil(delta_cast_number(DELTA_ARG0)));
}

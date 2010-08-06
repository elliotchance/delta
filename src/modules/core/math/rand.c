/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-rand.html
 * @brief Generate a random integer.
 */
DELTA_FUNCTION(rand)
{
	DELTA_RETURN_NUMBER(rand());
}

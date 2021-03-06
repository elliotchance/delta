/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Generate a random integer.
 */
DELTA_FUNCTION(rand)
{
	DELTA_RETURN_NUMBER(rand());
}

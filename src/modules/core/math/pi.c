/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Get value of pi.
 */
DELTA_FUNCTION(pi)
{
	DELTA_RETURN_NUMBER(M_PI);
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Calculate the length of the hypotenuse of a right-angle triangle.
 */
DELTA_FUNCTION(hypot)
{
	DELTA_RETURN_NUMBER(hypot(delta_cast_number(DELTA_ARG0), delta_cast_number(DELTA_ARG1)));
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-deg2rad.html
 * @category modules/core/math
 * 
 * @brief Converts the number in degrees to the radian equivalent.
 */
DELTA_FUNCTION(deg2rad)
{
	double number = delta_cast_number(DELTA_ARG0);
	DELTA_RETURN_NUMBER(number * (M_PI / 180.0));
}

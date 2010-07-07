/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Converts the number in degrees to the radian equivalent.
 */
delta_function(deg2rad)
{
	double number = delta_cast_number(DELTA_ARG0);
	DELTA_RETURN_NUMBER(number * (M_PI / 180.0));
}

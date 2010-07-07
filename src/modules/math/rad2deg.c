/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Converts the radian number to the equivalent number in degrees.
 */
delta_function(rad2deg)
{
	double number = delta_cast_number(DELTA_ARG0);
	DELTA_RETURN_NUMBER(number * (180.0 / M_PI));
}

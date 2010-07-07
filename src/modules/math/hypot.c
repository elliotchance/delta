/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Calculate the length of the hypotenuse of a right-angle triangle.
 */
delta_function(hypot)
{
	DELTA_RETURN_NUMBER(hypot(delta_cast_number(DELTA_ARG0), delta_cast_number(DELTA_ARG1)));
}

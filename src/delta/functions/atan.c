/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Arc tan (radians).
 */
delta_function(atan)
{
	DELTA_RETURN_NUMBER(atan(delta_cast_number(DELTA_ARG0)));
}

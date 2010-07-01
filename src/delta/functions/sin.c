/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Sine (radians).
 */
delta_function(sin)
{
	DELTA_RETURN_NUMBER(sin(delta_cast_number(DELTA_ARG0)));
}

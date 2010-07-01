/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Absolute value.
 */
delta_function(abs)
{
	DELTA_RETURN_NUMBER(abs(delta_cast_number(DELTA_ARG0)));
}

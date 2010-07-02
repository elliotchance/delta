/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Round down to the nearest integer.
 */
delta_function(floor)
{
	DELTA_RETURN_NUMBER(floor(delta_cast_number(DELTA_ARG0)));
}

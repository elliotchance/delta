/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Arc sine (radians).
 */
delta_function(asin)
{
	DELTA_RETURN_NUMBER(asin(delta_cast_number(DELTA_ARG0)));
}

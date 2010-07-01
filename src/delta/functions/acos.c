/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Arc cosine (radians).
 */
delta_function(acos)
{
	DELTA_RETURN_NUMBER(acos(delta_cast_number(DELTA_ARG0)));
}

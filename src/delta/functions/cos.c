/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Cosine (radians).
 */
delta_function(cos)
{
	DELTA_RETURN_NUMBER(cos(delta_cast_number(DELTA_ARG0)));
}

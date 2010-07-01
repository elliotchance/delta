/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Tangent (radians).
 */
delta_function(tan)
{
	DELTA_RETURN_NUMBER(tan(delta_cast_number(DELTA_ARG0)));
}

/*
 *  Delta programming language
 *
 *  Created by Elliot Chance on 25/06/10.
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

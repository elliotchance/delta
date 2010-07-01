/*
 *  Delta programming language
 *
 *  Created by Elliot Chance on 25/06/10.
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

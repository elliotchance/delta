/*
 *  Delta programming language
 *
 *  Created by Elliot Chance on 25/06/10.
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

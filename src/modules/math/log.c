/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Natural logarithm.
 */
delta_function(log)
{
	DELTA_RETURN_NUMBER(log(delta_cast_number(DELTA_ARG0)));
}

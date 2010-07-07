/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Base-10 logarithm.
 */
delta_function(log10)
{
	DELTA_RETURN_NUMBER(log10(delta_cast_number(DELTA_ARG0)));
}

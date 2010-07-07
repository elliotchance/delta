/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Round up to the nearest integer.
 */
delta_function(ceil)
{
	DELTA_RETURN_NUMBER(ceil(delta_cast_number(DELTA_ARG0)));
}

/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Square root.
 */
delta_function(sqrt)
{
	double temp = delta_cast_number(DELTA_ARG0);
	if(temp < 0)
		DELTA_RETURN_NAN;
	
	DELTA_RETURN_NUMBER(sqrt(temp));
}

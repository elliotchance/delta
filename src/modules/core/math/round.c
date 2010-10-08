/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 */
DELTA_FUNCTION(round)
{
	// round whole integer
	if(DELTA_ARGS == 1)
		DELTA_RETURN_NUMBER(round(delta_cast_number(DELTA_ARG0)));
	
	// round to places
	double r = delta_cast_number(DELTA_ARG0);
	r *= pow(10, delta_cast_int(DELTA_ARG1));
	r = round(r);
	r /= pow(10, delta_cast_int(DELTA_ARG1));
	DELTA_RETURN_NUMBER(r);
}

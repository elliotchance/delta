/*
 *  Delta programming language
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Square root.
 */
ins(SQT)
{
	double temp = delta_cast_number(DELTA_ARG0);
	if(temp < 0)
		DELTA_RETURN_NAN;
	
	DELTA_RETURN_NUMBER(sqrt(temp));
}

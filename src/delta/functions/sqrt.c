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
	//DELTA_PRINT_INS_ARGS(SQT);
	DELTA_RETURN_NUMBER(sqrt(delta_cast_number(DELTA_ARG0)));
}

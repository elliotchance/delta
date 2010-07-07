/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Seed the random number generator.
 */
delta_function(srand)
{
	srand(delta_cast_number(DELTA_ARG0));
	DELTA_RETURN_NULL;
}

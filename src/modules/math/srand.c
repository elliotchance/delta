/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @brief Seed the random number generator.
 */
DELTA_FUNCTION(srand)
{
	srand(delta_cast_number(DELTA_ARG0));
	DELTA_RETURN_NULL;
}

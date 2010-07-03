/*
 *  Delta programming language
 */

#include "ins.h"
#include <math.h>


/**
 * @brief Generate a random integer.
 */
delta_function(rand)
{
	DELTA_RETURN_NUMBER(rand());
}

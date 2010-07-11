/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @brief Show largest possible random value.
 * @syntax int getrandmax ( )
 *
 * Returns the maximum value that can be returned by a call to rand().
 *
 * @return The largest possible random value returned by rand().
 * @see rand
 * @see srand
 * @see mt_getrandmax
 */
DELTA_FUNCTION(getrandmax)
{
	DELTA_RETURN_NUMBER(RAND_MAX);
}

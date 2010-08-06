/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-mt_getrandmax.html
 * @category modules/core/math
 * 
 * @brief Show largest possible random value.
 * @syntax int mt_getrandmax ( )
 *
 * Returns the maximum value that can be returned by a call to mt_rand().
 *
 * @return Returns the maximum random value returned by mt_rand().
 * @see mt_rand
 * @see mt_srand
 * @see getrandmax
 */
DELTA_FUNCTION(mt_getrandmax)
{
	// the Mersenne twister will use the same max random as the standard rand()
	DELTA_RETURN_NUMBER(RAND_MAX);
}

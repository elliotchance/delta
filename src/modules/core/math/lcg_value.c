/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @page math-lcg_value.html
 * @brief Random number between 0 and 1.
 * @syntax number lcg_value ( )
 *
 * The true lcg_value() returns a pseudo random number in the range of (0, 1), combining two CGs
 * with periods of 2^31 - 85 and 2^31 - 249. The period is equal to the product of both primes.
 * However this implementation of the function only uses rand() and RAND_MAX to deliver a random
 * number between 0 and 1.
 *
 * @return Random number between 0 and 1.
 *
 * @see rand
 * @see mt_rand
 */
DELTA_FUNCTION(lcg_value)
{
	DELTA_RETURN_NUMBER((double) rand() / (double) RAND_MAX);
}

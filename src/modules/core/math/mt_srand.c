/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mt.h"
#include <sys/time.h>


/**
 * @category modules/core/math
 * 
 * @brief Seed the better random number generator.
 * @syntax void mt_srand ( )
 * @syntax void mt_srand ( int seed )
 * 
 * Seeds the random number generator with seed or with a random value if no seed is given.
 *
 * @param seed An optional seed value.
 * @return No value is returned.
 */
DELTA_FUNCTION(mt_srand)
{
	int seed;
	if(DELTA_ARGS == 0)
		seed = time(NULL);
	else
		seed = (int) delta_cast_number(DELTA_ARG0);
	
	srand(seed);
	int i;
    for(i = 0; i < MT_LEN; i++)
        mt_buffer[i] = rand();
    mt_index = 0;
	
	DELTA_RETURN_NUMBER(seed);
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <sys/time.h>


/**
 * @category modules/core/date
 */
DELTA_FUNCTION(microtime)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	
	if(DELTA_ARGS == 0 || !delta_cast_boolean(DELTA_ARG0)) {
		char *r = (char*) malloc(32);
		sprintf(r, "%.8f %d", tv.tv_usec / 1000000.0, (int) time(NULL));
		DELTA_RETURN_STRING(r);
	}
	
	DELTA_RETURN_NUMBER(time(NULL) + (tv.tv_usec / 1000000.0));
}

/*
 *  Delta programming language
 */

#include "ins.h"
#include <sys/time.h>


/**
 * @brief Return current Unix timestamp.
 * @syntax number time ( )
 *
 * Returns the current time measured in the number of seconds since the Unix Epoch (January 1 1970
 * 00:00:00 GMT).
 *
 * @see date
 * @see microtime
 */
delta_function(time)
{
	DELTA_RETURN_NUMBER(time(NULL));
}

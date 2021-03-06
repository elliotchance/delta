/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <sys/time.h>


/**
 * @category modules/core/date
 *
 * @brief Return current Unix timestamp.
 * @syntax number time ( )
 *
 * Returns the current time measured in the number of seconds since the Unix Epoch (January 1 1970
 * 00:00:00 GMT).
 *
 * @see date
 * @see microtime
 */
DELTA_FUNCTION(time)
{
	DELTA_RETURN_NUMBER(time(NULL));
}

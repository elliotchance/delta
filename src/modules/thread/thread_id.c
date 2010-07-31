/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


DELTA_FUNCTION(thread_id)
{
#ifdef DELTA_PLATFORM_MAC
	DELTA_RETURN_NUMBER((unsigned int) pthread_self());
#else
	DELTA_TRIGGER_ERROR("thread_id() is not supported on your platform.", DELTA_ERROR_ERROR);
#endif
	
	DELTA_RETURN_NULL;
}

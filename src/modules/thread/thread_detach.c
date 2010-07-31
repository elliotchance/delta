/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


DELTA_FUNCTION(thread_detach)
{
#ifdef DELTA_PLATFORM_MAC
	printf("detaching2!\n");
#else
	DELTA_TRIGGER_ERROR("thread_detach() is not supported on your platform.", DELTA_ERROR_ERROR);
#endif
	
	DELTA_RETURN_NULL;
}

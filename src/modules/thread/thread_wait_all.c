/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"


DELTA_FUNCTION(thread_wait_all)
{
	
#ifdef DELTA_PLATFORM_MAC
	
	// join all threads
	/*int i;
	for(i = 0; i < total_threads; ++i) {
		printf("joining");
		pthread_join(threads[i], NULL);
	}*/
	
	sleep(3);
	
#else
	
	DELTA_TRIGGER_ERROR("thread_wait_all() is not supported on your platform.", DELTA_ERROR_ERROR);
	DELTA_RETURN_NULL;
	
#endif
	
}

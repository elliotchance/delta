/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include <assert.h>


/**
 * @category modules/thread
 *
 * @brief Wait for all other threads to finish.
 */
DELTA_FUNCTION(thread_wait_all)
{
	
#ifdef DELTA_PLATFORM_MAC
	
	assert(!"thread_wait_all() is not working at the moment");
	
	// join all threads
	/*int i;
	for(i = 0; i < total_threads; ++i) {
		printf("joining");
		pthread_join(threads[i], NULL);
	}*/
	
	//sleep(3);
	
#else
	
	DELTA_TRIGGER_ERROR("thread_wait_all() is not supported on your platform.", DELTA_ERROR_ERROR);
	DELTA_RETURN_NULL;
	
#endif
	
}

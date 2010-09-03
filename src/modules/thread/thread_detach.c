/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"


/**
 * @category modules/thread
 *
 * @brief Detach a thread.
 */
DELTA_FUNCTION(thread_detach)
{
	
#ifdef DELTA_PLATFORM_MAC
	
	// check we have the VM
	if(DELTA_VM == NULL) {
		DELTA_TRIGGER_ERROR("thread_detach() can't get the virtual machine.", DELTA_ERROR_ERROR);
		DELTA_RETURN_NULL;
	}
	
	// make sure the function to be detached exists
	delta_jit_function func = delta_vm_get_function(DELTA_VM, "func");
	if(func == NULL) {
		DELTA_TRIGGER_ERROR("thread_detach() can't link function.", DELTA_ERROR_ERROR);
		DELTA_RETURN_NULL;
	}
	
	// run the function on a detached thread
	pthread_create(&threads[total_threads], NULL, (void*) func, NULL);
	++total_threads;
	
#else
	
	DELTA_TRIGGER_ERROR("thread_detach() is not supported on your platform.", DELTA_ERROR_ERROR);
	DELTA_RETURN_NULL;
	
#endif
	
}

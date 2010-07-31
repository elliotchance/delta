/*
 *  Delta programming language
 */

#include "module.h"


struct DeltaVM *DELTA_VM = NULL;


struct DeltaModuleFunction* module_functions(int *count)
{
	DELTA_PREPARE_MODULE_FUNCTIONS(10);
	
	DELTA_PUSH_FUNCTION(thread_detach, 1, 1);
	DELTA_PUSH_FUNCTION(thread_id, 0, 0);
	DELTA_PUSH_FUNCTION(thread_wait_all, 0, 0);
	
	DELTA_END_MODULE_FUNCTIONS;
}


void module_set_vm(struct DeltaVM *vm)
{
	DELTA_VM = vm;
}

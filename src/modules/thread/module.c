/*
 *  Delta programming language
 */

#include "module.h"


struct DeltaModuleFunction* module_functions(int *count)
{
	DELTA_PREPARE_MODULE_FUNCTIONS(10);
	
	DELTA_PUSH_FUNCTION(thread_detach, 0, 0);
	
	DELTA_END_MODULE_FUNCTIONS;
}

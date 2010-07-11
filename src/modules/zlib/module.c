/*
 *  Delta programming language
 */

#include "module.h"


struct DeltaModuleFunction* module_functions(int *count)
{
	DELTA_PREPARE_MODULE_FUNCTIONS(10);
	
	DELTA_PUSH_FUNCTION(gzopen, 2, 2);
	DELTA_PUSH_FUNCTION(gzclose, 1, 1);
	DELTA_PUSH_FUNCTION(gzeof, 1, 1);
	
	DELTA_END_MODULE_FUNCTIONS;
}

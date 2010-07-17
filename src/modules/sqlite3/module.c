/*
 *  Delta programming language
 */

#include "module.h"


struct DeltaModuleFunction* module_functions(int *count)
{
	DELTA_PREPARE_MODULE_FUNCTIONS(10);
	
	DELTA_PUSH_FUNCTION(sqlite3_open, 1, 1);
	DELTA_PUSH_FUNCTION(sqlite3_query, 2, 2);
	DELTA_PUSH_FUNCTION(sqlite3_fetch, 1, 1);
	
	DELTA_END_MODULE_FUNCTIONS;
}

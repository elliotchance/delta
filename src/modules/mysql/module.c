/*
 *  Delta programming language
 */

#include "module.h"


struct DeltaModuleFunction* module_functions(int *count)
{
	DELTA_PREPARE_MODULE_FUNCTIONS(10);
	
	DELTA_PUSH_FUNCTION(mysql_connect, 0, DELTA_MAX_ARGS);
	DELTA_PUSH_FUNCTION(mysql_query, 2, 2);
	DELTA_PUSH_FUNCTION(mysql_fetch_assoc, 1, 1);
	
	DELTA_END_MODULE_FUNCTIONS;
}

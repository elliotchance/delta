/*
 *  Delta programming language
 */

#include "module.h"


struct DeltaModuleFunction* module_functions(int *count)
{
	DELTA_PREPARE_MODULE_FUNCTIONS(20);
	
	DELTA_PUSH_FUNCTION(sqlite3_open, 1, 1);
	DELTA_PUSH_FUNCTION(sqlite3_query, 2, 2);
	DELTA_PUSH_FUNCTION(sqlite3_fetch, 1, 1);
	DELTA_PUSH_FUNCTION(sqlite3_close, 1, 1);
	DELTA_PUSH_FUNCTION(sqlite3_column_count, 1, 1);
	DELTA_PUSH_FUNCTION(sqlite3_column_name, 2, 2);
	DELTA_PUSH_FUNCTION(sqlite3_errcode, 1, 1);
	DELTA_PUSH_FUNCTION(sqlite3_extended_errcode, 1, 1);
	DELTA_PUSH_FUNCTION(sqlite3_errmsg, 1, 1);
	DELTA_PUSH_FUNCTION(sqlite3_exec, 2, 2);
	
	DELTA_PUSH_FUNCTION(sqlite3_finalize, 1, 1);
	DELTA_PUSH_FUNCTION(sqlite3_libversion, 1, 1);
	DELTA_PUSH_FUNCTION(sqlite3_sourceid, 1, 1);
	DELTA_PUSH_FUNCTION(sqlite3_libversion_number, 1, 1);
	DELTA_PUSH_FUNCTION(sqlite3_total_changes, 1, 1);
	
	DELTA_END_MODULE_FUNCTIONS;
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "sqlite3-external/sqlite3.h"


DELTA_FUNCTION(sqlite3_column_name)
{
	// check for valid resource
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_SQLITE3_QUERY)) {
		DELTA_TRIGGER_ERROR("Invalid resource type in sqlite3_column_name()", DELTA_ERROR_WARNING);
		DELTA_RETURN_NULL;
	}
	
	sqlite3_stmt *stmt = (sqlite3_stmt*) DELTA_ARG0->value.resource.ptr;
	
	DELTA_RETURN_STRING((char*) sqlite3_column_name(stmt, delta_cast_int(DELTA_ARG1)));
}

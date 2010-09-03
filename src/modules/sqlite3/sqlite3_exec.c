/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "sqlite3-external/sqlite3.h"


/**
 * @category modules/sqlite3
 *
 * @brief Execute query with no return handle.
 */
DELTA_FUNCTION(sqlite3_exec)
{
	// check for valid resource
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_SQLITE3_CONN)) {
		DELTA_TRIGGER_ERROR("Invalid resource type in sqlite3_exec()", DELTA_ERROR_WARNING);
		DELTA_RETURN_NULL;
	}
	
	char *sql = delta_cast_new_string(DELTA_ARG1, NULL);
	sqlite3 *db = (sqlite3*) DELTA_ARG0->value.resource.ptr;
	sqlite3_stmt *stmt = NULL;
	
	int result = sqlite3_exec(db, sql, NULL, NULL, NULL);
	
	free(sql);
	if(result == SQLITE_OK)
		DELTA_RETURN_RESOURCE(stmt, DELTA_RESOURCE_SQLITE3_QUERY);
	DELTA_RETURN_FALSE;
}

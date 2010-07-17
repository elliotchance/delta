/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "sqlite3-external/sqlite3.h"


DELTA_FUNCTION(sqlite3_query)
{
	// check for valid resource
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_SQLITE3_CONN)) {
		DELTA_TRIGGER_ERROR("Invalid resource type in sqlite3_query()", DELTA_ERROR_WARNING);
		DELTA_RETURN_NULL;
	}
	
	char *sql = delta_cast_new_string(DELTA_ARG1, NULL);
	sqlite3 *db = (sqlite3*) DELTA_ARG0->value.resource.ptr;
	sqlite3_stmt *stmt;
	
	int result = sqlite3_prepare(db, sql, -1, &stmt, NULL);
	
	free(sql);
	if(result == SQLITE_OK)
		DELTA_RETURN_RESOURCE(stmt, DELTA_RESOURCE_SQLITE3_QUERY);
	DELTA_RETURN_FALSE;
}

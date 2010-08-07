/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "sqlite3-external/sqlite3.h"


/**
 * @category modules/sqlite3
 *
 * @brief Fetch the next row in a query.
 */
DELTA_FUNCTION(sqlite3_fetch)
{
	// check for valid resource
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_SQLITE3_QUERY)) {
		DELTA_TRIGGER_ERROR("Invalid resource type in sqlite3_fetch()", DELTA_ERROR_WARNING);
		DELTA_RETURN_NULL;
	}
	
	sqlite3_stmt *stmt = (sqlite3_stmt*) DELTA_ARG0->value.resource.ptr;
	int result = sqlite3_step(stmt);
	
	if(result != SQLITE_ROW)
		DELTA_RETURN_FALSE;
	
	// convert the new row into an associative array
	int columns = sqlite3_column_count(stmt), i;
	struct DeltaArray r = delta_new_array();
	
	for(i = 0; i < columns; ++i)
		delta_array_push_k_string(&r, (char*) sqlite3_column_name(stmt, i),
								  (char*) sqlite3_column_text(stmt, i));
	
	DELTA_RETURN_ARRAY(r);
}

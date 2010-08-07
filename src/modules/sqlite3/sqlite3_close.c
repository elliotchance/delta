/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "sqlite3-external/sqlite3.h"


/**
 * @category modules/sqlite3
 *
 * @brief Close database handle.
 */
DELTA_FUNCTION(sqlite3_close)
{
	// check for valid resource
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_SQLITE3_CONN)) {
		DELTA_TRIGGER_ERROR("Invalid resource type in sqlite3_open()", DELTA_ERROR_WARNING);
		DELTA_RETURN_NULL;
	}
	
	sqlite3 *db = (sqlite3*) DELTA_ARG0->value.resource.ptr;
	int result = sqlite3_close(db);
	
	DELTA_RETURN_NUMBER(result);
}

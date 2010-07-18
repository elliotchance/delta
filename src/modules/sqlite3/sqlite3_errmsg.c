/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "sqlite3-external/sqlite3.h"


DELTA_FUNCTION(sqlite3_errmsg)
{
	// check for valid resource
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_SQLITE3_CONN)) {
		DELTA_TRIGGER_ERROR("Invalid resource type in sqlite3_errcode()", DELTA_ERROR_WARNING);
		DELTA_RETURN_NULL;
	}
	
	sqlite3 *db = (sqlite3*) DELTA_ARG0->value.resource.ptr;
	DELTA_RETURN_STRING((char*) sqlite3_errmsg(db));
}

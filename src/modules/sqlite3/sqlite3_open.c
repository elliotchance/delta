/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "sqlite3-external/sqlite3.h"


/**
 * @category modules/sqlite3
 *
 * @brief Open SQLite3 handle.
 */
DELTA_FUNCTION(sqlite3_open)
{
	char* arg0 = delta_cast_new_string(DELTA_ARG0, NULL);
	sqlite3 *res;
	sqlite3_open(arg0, &res);
	free(arg0);
	DELTA_RETURN_RESOURCE(res, DELTA_RESOURCE_SQLITE3_CONN);
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "sqlite3-external/sqlite3.h"


/**
 * @category modules/sqlite3
 *
 * @brief Source ID.
 */
DELTA_FUNCTION(sqlite3_sourceid)
{
	DELTA_RETURN_STRING((char*) sqlite3_sourceid());
}

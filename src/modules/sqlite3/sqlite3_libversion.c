/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "sqlite3-external/sqlite3.h"


/**
 * @category modules/sqlite3
 *
 * @brief Library version.
 */
DELTA_FUNCTION(sqlite3_libversion)
{
	DELTA_RETURN_STRING((char*) sqlite3_libversion());
}

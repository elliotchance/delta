/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "sqlite3-external/sqlite3.h"


DELTA_FUNCTION(sqlite3_libversion)
{
	DELTA_RETURN_STRING((char*) sqlite3_libversion());
}
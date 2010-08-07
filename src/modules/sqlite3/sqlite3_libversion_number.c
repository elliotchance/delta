/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "sqlite3-external/sqlite3.h"


/**
 * @category modules/sqlite3
 *
 * @brief Library version number.
 */
DELTA_FUNCTION(sqlite3_libversion_number)
{
	DELTA_RETURN_NUMBER(sqlite3_libversion_number());
}

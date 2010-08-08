/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @category modules/core/string
 * 
 * @brief Returns a one-character string containing the character specified by ascii.
 * @syntax string chr ( int ascii )
 *
 * @param ascii The ascii code.
 * @return Returns the specified character.
 * @see chr
 */
DELTA_FUNCTION(chr)
{
	DELTA_RETURN_CHAR(delta_cast_number(DELTA_ARG0));
}

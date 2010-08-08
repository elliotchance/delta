/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <string.h>


/**
 * @category modules/core/string
 * 
 * @brief Returns the ASCII value of the first character of string.
 * @syntax int ord ( string string )
 *
 * @param string A character.
 * @return Returns the ASCII value as an integer.
 * @see chr
 */
DELTA_FUNCTION(ord)
{
	DELTA_RETURN_NUMBER((double) delta_cast_char(DELTA_ARG0));
}

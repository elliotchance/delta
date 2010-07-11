/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @brief Returns TRUE if the argument is an array.
 */
DELTA_FUNCTION(is_array)
{
	DELTA_RETURN_BOOLEAN(DELTA_ARG0->type == DELTA_TYPE_ARRAY);
}

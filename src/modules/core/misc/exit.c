/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @category modules/core/misc
 * 
 * @brief Terminate the current script.
 * @syntax void exit ( int status )
 */
DELTA_FUNCTION(exit)
{
	exit(delta_cast_int(DELTA_ARG0));
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"


/**
 * @brief Flushes the output to a file.
 * @syntax boolean fflush ( resource handle )
 *
 * This function forces a write of all buffered output to the resource pointed to by the file
 * handle.
 *
 * @param handle The file pointer must be valid, and must point to a file successfully opened by
 *        fopen() or fsockopen() (and not yet closed by fclose()).
 * @return Returns TRUE on success or FALSE on failure.
 */
DELTA_FUNCTION(fflush)
{
	// check for correct resource type
	if(!DELTA_CHECK_RESOURCE(ram[DELTA_ARG0], DELTA_RESOURCE_FILE)) {
		// TODO: push warning here
		DELTA_RETURN_FALSE;
	}
	
	// check for eof
	int r = fflush((FILE*) ram[DELTA_ARG0]->value.resource.ptr);
	DELTA_RETURN_BOOLEAN(r);
}

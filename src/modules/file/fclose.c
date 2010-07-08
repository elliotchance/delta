/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"


/**
 * @brief Close a file handle.
 * @syntax boolean fclose ( resource handle )
 */
DELTA_FUNCTION(fclose)
{
	// check for correct resource type
	if(!DELTA_CHECK_RESOURCE(ram[DELTA_ARG0], DELTA_RESOURCE_FILE)) {
		// TODO: push warning here
		DELTA_RETURN_ZERO;
	}
	
	// close pointer
	int r = fclose((FILE*) ram[DELTA_ARG0]->value.resource.ptr);
	DELTA_RETURN_BOOLEAN(r);
}

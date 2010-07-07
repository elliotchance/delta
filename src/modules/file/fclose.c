/*
 *  Delta programming language
 */

#include "ins.h"
#include "file.h"


/**
 * @brief Close a file handle.
 * @syntax boolean fclose ( resource handle )
 */
delta_function(fclose)
{
	// check for correct resource type
	if(ram[DELTA_ARG0]->type != DELTA_TYPE_RESOURCE ||
	   ram[DELTA_ARG0]->value.resource.id != DELTA_RESOURCE_FILE) {
		// TODO: push warning here
		DELTA_RETURN_ZERO;
	}
	
	// close pointer
	int r = fclose((FILE*) ram[DELTA_ARG0]->value.resource.ptr);
	DELTA_RETURN_BOOLEAN(r);
}

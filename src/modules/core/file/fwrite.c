/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "../module.h"


/**
 * @category modules/core/file
 *
 * @brief Write to a file handle.
 * @syntax int fwrite ( resource handle , string string [, int length ] )
 */
DELTA_FUNCTION(fwrite)
{
	// check for correct resource type
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_FILE)) {
		DELTA_TRIGGER_ERROR("Resource to fwrite() is not valid", DELTA_ERROR_WARNING);
		DELTA_RETURN_ZERO;
	}
	
	// TODO: length isn't supported yet
	int release1;
	struct DeltaVariable *arg1 = delta_cast_string(DELTA_ARG1, &release1);
	
	// create the resource
	int r = fwrite(arg1->value.ptr, arg1->size, 1, (FILE*) DELTA_ARG0->value.resource.ptr);
	
	// clean up
	DELTA_RELEASE(release1, arg1);
	DELTA_RETURN_NUMBER(r);
}

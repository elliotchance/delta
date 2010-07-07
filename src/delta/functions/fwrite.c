/*
 *  Delta programming language
 */

#include "ins.h"
#include "../../modules/file.h"


/**
 * @brief Write to a file handle.
 * @syntax int fwrite ( resource handle , string string [, int length ] )
 */
delta_function(fwrite)
{
	// check for correct resource type
	if(ram[DELTA_ARG0]->type != DELTA_TYPE_RESOURCE ||
	   ram[DELTA_ARG0]->value.resource.id != DELTA_RESOURCE_FILE) {
		// TODO: push warning here
		DELTA_RETURN_ZERO;
	}
	
	// TODO: length isnt supported yet
	int release1;
	struct DeltaVariable *arg1 = delta_cast_string(DELTA_ARG1, &release1);
	
	// create the resource
	int r = fwrite(arg1->value.ptr, arg1->size, 1, (FILE*) ram[DELTA_ARG0]->value.resource.ptr);
	
	// clean up
	if(release1)
		free(arg1);
	
	DELTA_RETURN_NUMBER(r);
}

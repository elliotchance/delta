/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "../module.h"


/**
 * @brief Opens file handle.
 * @syntax resource fopen ( string filename , string mode )
 */
DELTA_FUNCTION(fopen)
{
	int release0, release1;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	struct DeltaVariable *arg1 = delta_cast_string(DELTA_ARG1, &release1);
	
	// create the resource
	FILE* f = fopen(arg0->value.ptr, arg1->value.ptr);
	
	// clean up
	DELTA_RELEASE(release0, arg0);
	DELTA_RELEASE(release1, arg1);
	
	if(f == NULL)
		DELTA_RETURN_NULL;
	DELTA_RETURN_RESOURCE(f, DELTA_RESOURCE_FILE);
}

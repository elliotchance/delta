/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "zlib-external/zlib.h"


DELTA_FUNCTION(gzeof)
{
	// check for correct resource type
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_GZFILE)) {
		DELTA_TRIGGER_ERROR("Resource to gzeof() is not valid", DELTA_ERROR_WARNING);
		DELTA_RETURN_ZERO;
	}
	
	DELTA_RETURN_BOOLEAN(gzeof(DELTA_ARG0->value.resource.ptr));
}

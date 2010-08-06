/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "zlib-external/zlib.h"


/**
 * @page zlib-gzclose.html
 * @brief Close an open gz-file pointer.
 * @syntax boolean gzclose ( resource zp )
 *
 * Closes the given gz-file pointer.
 *
 * @param zp The gz-file pointer. It must be valid, and must point to a file successfully opened by
 *        gzopen().
 * @return Returns TRUE on success or FALSE on failure.
 */
DELTA_FUNCTION(gzclose)
{
	// check for correct resource type
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_GZFILE)) {
		DELTA_TRIGGER_ERROR("Resource to gzclose() is not valid", DELTA_ERROR_WARNING);
		DELTA_RETURN_ZERO;
	}
	
	DELTA_RETURN_BOOLEAN(gzclose(DELTA_ARG0->value.resource.ptr));
}

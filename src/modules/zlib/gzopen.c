/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "zlib-external/zlib.h"


/**
 * @brief Open gz-file.
 */
DELTA_FUNCTION(gzopen)
{
	int release0, release1;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	struct DeltaVariable *arg1 = delta_cast_string(DELTA_ARG1, &release1);
	
	gzFile r = gzopen(arg0->value.ptr, arg1->value.ptr);
	
	DELTA_RELEASE(release0, arg0);
	DELTA_RELEASE(release1, arg1);
	DELTA_RETURN_RESOURCE(r, DELTA_RESOURCE_GZFILE);
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"


/**
 * @brief Gets character from file pointer.
 * @syntax string fgetc ( resource handle )
 *
 * This function forces a write of all buffered output to the resource pointed to by the file
 * handle.
 *
 * @param handle The file pointer must be valid, and must point to a file successfully opened by
 *        fopen() or fsockopen() (and not yet closed by fclose()).
 * @return Returns a string containing a single character read from the file pointed to by handle.
 *         Returns FALSE on EOF.
 */
DELTA_FUNCTION(fgetc)
{
	// check for correct resource type
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_FILE)) {
		DELTA_TRIGGER_ERROR("Resource to fgetc() is not valid", DELTA_ERROR_WARNING);
		DELTA_RETURN_FALSE;
	}
	
	// check for eof
	int r = fgetc((FILE*) DELTA_ARG0->value.resource.ptr);
	DELTA_RETURN_STRING((char*) &r);
}

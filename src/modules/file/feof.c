/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"


/**
 * @brief Tests for end-of-file on a file pointer.
 * @syntax boolean feof ( resource handle )
 *
 * Tests for end-of-file on a file pointer.
 *
 * @param handle The file pointer must be valid, and must point to a file successfully opened by
 *        fopen() or fsockopen() (and not yet closed by fclose()).
 * @return Returns TRUE if the file pointer is at EOF or an error occurs (including socket timeout);
 *         otherwise returns FALSE.
 */
DELTA_FUNCTION(feof)
{
	// check for correct resource type
	if(!DELTA_CHECK_RESOURCE(ram[DELTA_ARG0], DELTA_RESOURCE_FILE)) {
		// TODO: push warning here
		DELTA_RETURN_TRUE;
	}
	
	// check for eof
	int r = feof((FILE*) ram[DELTA_ARG0]->value.resource.ptr);
	DELTA_RETURN_BOOLEAN(r);
}

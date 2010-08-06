/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "../module.h"


/**
 * @page file-fgets.html
 * @category modules/core/file
 *
 * @brief Gets line from file pointer.
 * @syntax string fgets ( resource handle [, int length ] )
 *
 * Gets a line from file pointer.
 *
 * @param handle The file pointer must be valid, and must point to a file successfully opened by
 *        fopen() or fsockopen() (and not yet closed by fclose()).
 * @param length Reading ends when length - 1 bytes have been read, on a newline (which is included
 *        in the return value), or on EOF (whichever comes first). If no length is specified, it
 *        will keep reading from the stream until it reaches the end of the line.
 * @return Returns a string of up to length - 1 bytes read from the file pointed to by handle. If an
 *         error occurs, returns FALSE.
 */
DELTA_FUNCTION(fgets)
{
	// check for correct resource type
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_FILE)) {
		DELTA_TRIGGER_ERROR("Resource to fgets() is not valid", DELTA_ERROR_WARNING);
		DELTA_RETURN_FALSE;
	}
	
	int len = 4097;
	if(DELTA_ARGS > 1)
		len = delta_cast_number(DELTA_ARG1);
	char *buf = (char*) malloc(len);
	
	fgets(buf, len - 1, (FILE*) DELTA_ARG0->value.resource.ptr);
	DELTA_RETURN_STRING(buf);
}

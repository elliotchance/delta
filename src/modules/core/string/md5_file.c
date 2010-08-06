/*
 *  Delta programming language
 */

#include "delta/delta.h"


// from md5.c
char* MDFile(char *filename);


/**
 * @page string-md5_file.html
 * @brief Get the MD5 of a string.
 */
DELTA_FUNCTION(md5_file)
{
	int release;
	struct DeltaVariable *v = delta_cast_string(DELTA_ARG0, &release);
	char *md5_result = MDFile(v->value.ptr);
	
	DELTA_RELEASE(release, v);
	DELTA_RETURN_STRING(md5_result);
}

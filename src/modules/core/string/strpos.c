/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <string.h>


DELTA_FUNCTION(strpos)
{
	int release0, release1;
	struct DeltaVariable *haystack = delta_cast_string(DELTA_ARG0, &release0);
	struct DeltaVariable *needle = delta_cast_string(DELTA_ARG1, &release1);
	
	char *p = strstr(haystack->value.ptr, needle->value.ptr);
	int location = -1;
	if(p)
		location = p - haystack->value.ptr;
	
	DELTA_RELEASE(release0, haystack);
	DELTA_RELEASE(release1, needle);
	
	if(location < 0)
		DELTA_RETURN_FALSE;
	DELTA_RETURN_NUMBER(location);
}

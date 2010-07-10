/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"


DELTA_FUNCTION(apm_lib_version)
{
	char *lib_version = (char*) malloc(80);
	m_apm_lib_version(lib_version);
	DELTA_RETURN_STRING(lib_version);
}

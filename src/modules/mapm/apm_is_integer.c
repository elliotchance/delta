/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"
#include "module.h"


DELTA_FUNCTION(apm_is_integer)
{
	DELTA_RETURN_BOOLEAN(m_apm_is_integer(apm_get(DELTA_ARG0)));
}

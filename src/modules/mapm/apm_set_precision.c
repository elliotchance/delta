/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"
#include "module.h"


DELTA_FUNCTION(apm_set_precision)
{
	mapm_precision = delta_cast_number(DELTA_ARG0);
	DELTA_RETURN_NUMBER(mapm_precision);
}

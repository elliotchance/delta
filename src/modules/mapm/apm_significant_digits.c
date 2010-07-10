/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"
#include "module.h"


DELTA_FUNCTION(apm_significant_digits)
{
	DELTA_RETURN_NUMBER(m_apm_significant_digits(apm_get(DELTA_ARG0)));
}

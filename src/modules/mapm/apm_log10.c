/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"
#include "module.h"


/**
 * @category modules/mapm
 *
 * @brief Base-10 logarithm.
 */
DELTA_FUNCTION(apm_log10)
{
	// do calculation
	M_APM r = m_apm_init(), temp = m_apm_init(), arg0 = apm_get(DELTA_ARG0);
	m_apm_log10(temp, delta_cast_number(DELTA_ARG1), arg0);
	
	// return
	char *str = (char*) malloc(mapm_precision + 8);
	m_apm_round(r, mapm_precision, temp);
	m_apm_free(arg0);
	m_apm_free(temp);
	m_apm_to_fixpt_string(str, mapm_precision, r);
	DELTA_RETURN_STRING(str);
}

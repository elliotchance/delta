/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"
#include "module.h"


/**
 * @category modules/mapm
 *
 * @brief Power.
 */
DELTA_FUNCTION(apm_pow)
{
	// TODO: maybe use m_apm_integer_pow() where possible, it might be faster
	
	// do calculation
	M_APM r = m_apm_init(), temp = m_apm_init();
	M_APM arg0 = apm_get(DELTA_ARG0), arg1 = apm_get(DELTA_ARG1);
	m_apm_pow(temp, mapm_precision, arg0, arg1);
	
	// return
	char *str = (char*) malloc(mapm_precision + 8);
	m_apm_round(r, mapm_precision, temp);
	m_apm_free(arg0);
	m_apm_free(arg1);
	m_apm_free(temp);
	m_apm_to_fixpt_string(str, mapm_precision, r);
	DELTA_RETURN_STRING(str);
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"
#include "module.h"


/**
 * @category modules/mapm
 *
 * @brief Get random value.
 */
DELTA_FUNCTION(apm_get_random)
{
	M_APM random_value = m_apm_init(), r = m_apm_init();
	m_apm_get_random(random_value);
	
	// return
	char *str = (char*) malloc(mapm_precision + 8);
	m_apm_round(r, mapm_precision, random_value);
	m_apm_to_fixpt_string(str, mapm_precision, r);
	DELTA_RETURN_STRING(str);
}

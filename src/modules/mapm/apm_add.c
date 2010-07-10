/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"


DELTA_FUNCTION(apm_add)
{
	// prepare input
	M_APM m1 = m_apm_init(), m2 = m_apm_init(), m3 = m_apm_init();
	m_apm_set_double(m1, delta_cast_number(DELTA_ARG0));
	m_apm_set_double(m2, delta_cast_number(DELTA_ARG1));
	
	// do calculation
	m_apm_add(m3, m1, m2);
	
	// return
	char *r = (char*) malloc(32);
	m_apm_to_fixpt_string(r, -1, m3);
	DELTA_RETURN_STRING(r);
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"
#include "module.h"


/**
 * @category modules/mapm
 *
 * @brief Exponent.
 */
DELTA_FUNCTION(apm_exponent)
{
	DELTA_RETURN_NUMBER(m_apm_exponent(apm_get(DELTA_ARG0)));
}

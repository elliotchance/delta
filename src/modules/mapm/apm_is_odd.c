/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"
#include "module.h"


/**
 * @category modules/mapm
 *
 * @brief Is odd.
 */
DELTA_FUNCTION(apm_is_odd)
{
	DELTA_RETURN_BOOLEAN(m_apm_is_odd(apm_get(DELTA_ARG0)));
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"
#include "module.h"
#include "delta/compiler/strings.h"


DELTA_FUNCTION(apm_set_random_seed)
{
	int release;
	struct DeltaVariable *v = delta_cast_string(DELTA_ARG0, &release);
	
	if(release)
		m_apm_set_random_seed(v->value.ptr);
	else
		m_apm_set_random_seed(delta_copy_string(v->value.ptr));
	
	DELTA_RETURN_TRUE;
}

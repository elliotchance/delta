/*
 *  Delta programming language
 */

#include "module.h"
#include "delta/compiler/strings.h"


int mapm_precision = 16;


struct DeltaModuleFunction* module_functions(int *count)
{
	DELTA_PREPARE_MODULE_FUNCTIONS(50);
	
	DELTA_PUSH_FUNCTION(apm_add, 2, 2);
	DELTA_PUSH_FUNCTION(apm_set_precision, 1, 1);
	DELTA_PUSH_FUNCTION(apm_lib_version, 0, 0);
	DELTA_PUSH_FUNCTION(apm_lib_short_version, 0, 0);
	DELTA_PUSH_FUNCTION(apm_absolute_value, 1, 1);
	DELTA_PUSH_FUNCTION(apm_negate, 1, 1);
	DELTA_PUSH_FUNCTION(apm_arctanh, 1, 1);
	DELTA_PUSH_FUNCTION(apm_arcsinh, 1, 1);
	DELTA_PUSH_FUNCTION(apm_arccosh, 1, 1);
	DELTA_PUSH_FUNCTION(apm_tanh, 1, 1);
	
	DELTA_PUSH_FUNCTION(apm_sinh, 1, 1);
	DELTA_PUSH_FUNCTION(apm_cosh, 1, 1);
	DELTA_PUSH_FUNCTION(apm_arctan2, 2, 2);
	DELTA_PUSH_FUNCTION(apm_arctan, 1, 1);
	DELTA_PUSH_FUNCTION(apm_arcsin, 1, 1);
	DELTA_PUSH_FUNCTION(apm_arccos, 1, 1);
	DELTA_PUSH_FUNCTION(apm_tan, 1, 1);
	DELTA_PUSH_FUNCTION(apm_sin, 1, 1);
	DELTA_PUSH_FUNCTION(apm_cos, 1, 1);
	DELTA_PUSH_FUNCTION(apm_log10, 1, 1);
	
	DELTA_PUSH_FUNCTION(apm_pow, 2, 2);
	DELTA_PUSH_FUNCTION(apm_log, 1, 1);
	DELTA_PUSH_FUNCTION(apm_exp, 1, 1);
	DELTA_PUSH_FUNCTION(apm_cbrt, 1, 1);
	DELTA_PUSH_FUNCTION(apm_sqrt, 1, 1);
	DELTA_PUSH_FUNCTION(apm_lcm, 2, 2);
	DELTA_PUSH_FUNCTION(apm_gcd, 2, 2);
	DELTA_PUSH_FUNCTION(apm_floor, 1, 1);
	DELTA_PUSH_FUNCTION(apm_ceil, 1, 1);
	DELTA_PUSH_FUNCTION(apm_reciprocal, 1, 1);
	
	DELTA_PUSH_FUNCTION(apm_factorial, 1, 1);
	DELTA_PUSH_FUNCTION(apm_divide, 2, 2);
	DELTA_PUSH_FUNCTION(apm_subtract, 2, 2);
	DELTA_PUSH_FUNCTION(apm_multiply, 2, 2);
	DELTA_PUSH_FUNCTION(apm_get_random, 0, 0);
	DELTA_PUSH_FUNCTION(apm_set_random_seed, 1, 1);
	DELTA_PUSH_FUNCTION(apm_is_odd, 1, 1);
	DELTA_PUSH_FUNCTION(apm_is_even, 1, 1);
	DELTA_PUSH_FUNCTION(apm_is_integer, 1, 1);
	DELTA_PUSH_FUNCTION(apm_significant_digits, 1, 1);
	
	DELTA_PUSH_FUNCTION(apm_sign, 1, 1);
	DELTA_PUSH_FUNCTION(apm_exponent, 1, 1);
	DELTA_PUSH_FUNCTION(apm_round, 1, 1);
	
	DELTA_END_MODULE_FUNCTIONS;
	return f;
}


M_APM apm_get(struct DeltaVariable *v)
{
	M_APM r = m_apm_init();
	
	if(v->type == DELTA_TYPE_NUMBER)
		m_apm_set_double(r, delta_cast_number(v));
	else {
		int release;
		struct DeltaVariable *arg = delta_cast_string(v, &release);
		
		if(release)
			m_apm_set_string(r, arg->value.ptr);
		else
			m_apm_set_string(r, delta_copy_string(arg->value.ptr));
	}
	
	return r;
}

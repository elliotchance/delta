/*
 *  Delta programming language
 */

#include "module.h"
#include "delta/compiler/strings.h"


// prototypes here so we dont have to include more files
int delta_vm_push_function(struct DeltaFunction* f);
struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args);

int mapm_precision = 16;


void delta_load_module_mapm()
{
	delta_vm_push_function(new_DeltaFunction("apm_add",                FUNC(apm_add), 2, 2));
	delta_vm_push_function(new_DeltaFunction("apm_set_precision",      FUNC(apm_set_precision), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_lib_version",        FUNC(apm_lib_version), 0, 0));
	delta_vm_push_function(new_DeltaFunction("apm_lib_short_version",  FUNC(apm_lib_short_version), 0, 0));
	delta_vm_push_function(new_DeltaFunction("apm_absolute_value",     FUNC(apm_absolute_value), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_negate",             FUNC(apm_negate), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_arctanh",            FUNC(apm_arctanh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_arcsinh",            FUNC(apm_arcsinh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_arccosh",            FUNC(apm_arccosh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_tanh",               FUNC(apm_tanh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_sinh",               FUNC(apm_sinh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_cosh",               FUNC(apm_cosh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_arctan2",            FUNC(apm_arctan2), 2, 2));
	delta_vm_push_function(new_DeltaFunction("apm_arctan",             FUNC(apm_arctan), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_arcsin",             FUNC(apm_arcsin), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_arccos",             FUNC(apm_arccos), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_tan",                FUNC(apm_tan), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_sin",                FUNC(apm_sin), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_cos",                FUNC(apm_cos), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_log10",              FUNC(apm_log10), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_pow",                FUNC(apm_pow), 2, 2));
	delta_vm_push_function(new_DeltaFunction("apm_log",                FUNC(apm_log), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_exp",                FUNC(apm_exp), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_cbrt",               FUNC(apm_cbrt), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_sqrt",               FUNC(apm_sqrt), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_lcm",                FUNC(apm_lcm), 2, 2));
	delta_vm_push_function(new_DeltaFunction("apm_gcd",                FUNC(apm_gcd), 2, 2));
	delta_vm_push_function(new_DeltaFunction("apm_floor",              FUNC(apm_floor), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_ceil",               FUNC(apm_ceil), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_reciprocal",         FUNC(apm_reciprocal), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_factorial",          FUNC(apm_factorial), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_divide",             FUNC(apm_divide), 2, 2));
	delta_vm_push_function(new_DeltaFunction("apm_subtract",           FUNC(apm_subtract), 2, 2));
	delta_vm_push_function(new_DeltaFunction("apm_multiply",           FUNC(apm_multiply), 2, 2));
	delta_vm_push_function(new_DeltaFunction("apm_get_random",         FUNC(apm_get_random), 0, 0));
	delta_vm_push_function(new_DeltaFunction("apm_set_random_seed",    FUNC(apm_set_random_seed), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_is_odd",             FUNC(apm_is_odd), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_is_even",            FUNC(apm_is_even), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_is_integer",         FUNC(apm_is_integer), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_significant_digits", FUNC(apm_significant_digits), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_sign",               FUNC(apm_sign), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_exponent",           FUNC(apm_exponent), 1, 1));
	delta_vm_push_function(new_DeltaFunction("apm_round",              FUNC(apm_round), 1, 1));
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

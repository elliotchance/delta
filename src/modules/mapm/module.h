/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"


extern int mapm_precision;


void  delta_load_module_mapm();
M_APM apm_get(struct DeltaVariable *v);


DELTA_FUNCTION(apm_add);
DELTA_FUNCTION(apm_set_precision);
DELTA_FUNCTION(apm_lib_version);
DELTA_FUNCTION(apm_lib_short_version);
DELTA_FUNCTION(apm_absolute_value);
DELTA_FUNCTION(apm_negate);
DELTA_FUNCTION(apm_arctanh);
DELTA_FUNCTION(apm_arcsinh);
DELTA_FUNCTION(apm_arccosh);
DELTA_FUNCTION(apm_tanh);
DELTA_FUNCTION(apm_sinh);
DELTA_FUNCTION(apm_cosh);
DELTA_FUNCTION(apm_arctan2);
DELTA_FUNCTION(apm_arctan);
DELTA_FUNCTION(apm_arcsin);
DELTA_FUNCTION(apm_arccos);
DELTA_FUNCTION(apm_tan);
DELTA_FUNCTION(apm_sin);
DELTA_FUNCTION(apm_cos);
DELTA_FUNCTION(apm_log10);
DELTA_FUNCTION(apm_pow);
DELTA_FUNCTION(apm_log);
DELTA_FUNCTION(apm_exp);
DELTA_FUNCTION(apm_cbrt);
DELTA_FUNCTION(apm_sqrt);
DELTA_FUNCTION(apm_lcm);
DELTA_FUNCTION(apm_gcd);
DELTA_FUNCTION(apm_floor);
DELTA_FUNCTION(apm_ceil);
DELTA_FUNCTION(apm_reciprocal);
DELTA_FUNCTION(apm_factorial);
DELTA_FUNCTION(apm_divide);
DELTA_FUNCTION(apm_subtract);
DELTA_FUNCTION(apm_multiply);
DELTA_FUNCTION(apm_get_random);
DELTA_FUNCTION(apm_set_random_seed);
DELTA_FUNCTION(apm_is_odd);
DELTA_FUNCTION(apm_is_even);
DELTA_FUNCTION(apm_is_integer);
DELTA_FUNCTION(apm_significant_digits);
DELTA_FUNCTION(apm_sign);
DELTA_FUNCTION(apm_exponent);
DELTA_FUNCTION(apm_round);

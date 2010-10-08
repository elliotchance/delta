/*
 *  Delta programming language
 */

#include "module.h"


struct DeltaModuleFunction* module_functions(int *count)
{
	DELTA_PREPARE_MODULE_FUNCTIONS(160);
	
	// array
	DELTA_PUSH_FUNCTION(array, 0, DELTA_MAX_ARGS);
	DELTA_PUSH_FUNCTION(array_key_exists, 2, 2);
	DELTA_PUSH_FUNCTION(array_keys, 1, 1);
	DELTA_PUSH_FUNCTION(array_push, 3, 3);
	DELTA_PUSH_FUNCTION(array_sum, 0, DELTA_MAX_ARGS);
	DELTA_PUSH_FUNCTION(array_value_exists, 2, 2);
	DELTA_PUSH_FUNCTION(array_values, 1, 1);
	DELTA_PUSH_FUNCTION(count, 1, 1);
	DELTA_PUSH_FUNCTION(array_pop, 1, 1);
	DELTA_PUSH_FUNCTION(split, 2, 2);
	
	// ctype
	DELTA_PUSH_FUNCTION(ctype_alnum, 1, 1);
	DELTA_PUSH_FUNCTION(ctype_alpha, 1, 1);
	DELTA_PUSH_FUNCTION(ctype_cntrl, 1, 1);
	DELTA_PUSH_FUNCTION(ctype_digit, 1, 1);
	DELTA_PUSH_FUNCTION(ctype_graph, 1, 1);
	DELTA_PUSH_FUNCTION(ctype_lower, 1, 1);
	DELTA_PUSH_FUNCTION(ctype_print, 1, 1);
	DELTA_PUSH_FUNCTION(ctype_punct, 1, 1);
	DELTA_PUSH_FUNCTION(ctype_space, 1, 1);
	DELTA_PUSH_FUNCTION(ctype_upper, 1, 1);
	
	DELTA_PUSH_FUNCTION(ctype_xdigit, 1, 1);
	
	// date
	DELTA_PUSH_FUNCTION(time, 0, 0);
	DELTA_PUSH_FUNCTION(microtime, 0, 1);
	
	// error
	DELTA_PUSH_FUNCTION(trigger_error, 2, 2);
	
	// file
	DELTA_PUSH_FUNCTION(fclose, 2, 2);
	DELTA_PUSH_FUNCTION(feof, 2, 2);
	DELTA_PUSH_FUNCTION(fflush, 2, 2);
	DELTA_PUSH_FUNCTION(fgetc, 2, 2);
	DELTA_PUSH_FUNCTION(fgets, 2, 2);
	DELTA_PUSH_FUNCTION(fopen, 2, 2);
	DELTA_PUSH_FUNCTION(fwrite, 2, 2);
	
	// math
	DELTA_PUSH_FUNCTION(abs, 1, 1);
	DELTA_PUSH_FUNCTION(acos, 1, 1);
	DELTA_PUSH_FUNCTION(acosh, 1, 1);
	DELTA_PUSH_FUNCTION(asin, 1, 1);
	DELTA_PUSH_FUNCTION(asinh, 1, 1);
	DELTA_PUSH_FUNCTION(atan, 1, 1);
	DELTA_PUSH_FUNCTION(atan2, 2, 2);
	DELTA_PUSH_FUNCTION(atanh, 1, 1);
	DELTA_PUSH_FUNCTION(base_convert, 3, 3);
	DELTA_PUSH_FUNCTION(bindec, 1, 1);
	
	DELTA_PUSH_FUNCTION(ceil, 1, 1);
	DELTA_PUSH_FUNCTION(cos, 1, 1);
	DELTA_PUSH_FUNCTION(cosh, 1, 1);
	DELTA_PUSH_FUNCTION(decbin, 1, 1);
	DELTA_PUSH_FUNCTION(dechex, 1, 1);
	DELTA_PUSH_FUNCTION(decoct, 1, 1);
	DELTA_PUSH_FUNCTION(deg2rad, 1, 1);
	DELTA_PUSH_FUNCTION(exp, 1, 1);
	DELTA_PUSH_FUNCTION(expm1, 1, 1);
	DELTA_PUSH_FUNCTION(floor, 1, 1);
	
	DELTA_PUSH_FUNCTION(fmod, 2, 2);
	DELTA_PUSH_FUNCTION(getrandmax, 0, 0);
	DELTA_PUSH_FUNCTION(hexdec, 1, 1);
	DELTA_PUSH_FUNCTION(hypot, 2, 2);
	DELTA_PUSH_FUNCTION(lcg_value, 1, 1);
	DELTA_PUSH_FUNCTION(log, 1, 1);
	DELTA_PUSH_FUNCTION(log1p, 1, 1);
	DELTA_PUSH_FUNCTION(log10, 1, 1);
	DELTA_PUSH_FUNCTION(max, 1, DELTA_MAX_ARGS);
	DELTA_PUSH_FUNCTION(min, 1, DELTA_MAX_ARGS);
	
	DELTA_PUSH_FUNCTION(mt_getrandmax, 0, 0);
	DELTA_PUSH_FUNCTION(mt_rand, 0, 0);
	DELTA_PUSH_FUNCTION(mt_srand, 1, 1);
	DELTA_PUSH_FUNCTION(octdec, 1, 1);
	DELTA_PUSH_FUNCTION(pi, 0, 0);
	DELTA_PUSH_FUNCTION(pow, 2, 2);
	DELTA_PUSH_FUNCTION(rad2deg, 1, 1);
	DELTA_PUSH_FUNCTION(rand, 0, 0);
	DELTA_PUSH_FUNCTION(sin, 1, 1);
	DELTA_PUSH_FUNCTION(sinh, 1, 1);
	
	DELTA_PUSH_FUNCTION(sqrt, 1, 1);
	DELTA_PUSH_FUNCTION(srand, 1, 1);
	DELTA_PUSH_FUNCTION(tan, 1, 1);
	DELTA_PUSH_FUNCTION(tanh, 1, 1);
	DELTA_PUSH_FUNCTION(round, 1, 2);
	
	// misc
	DELTA_PUSH_FUNCTION(exit, 1, 1);
	DELTA_PUSH_FUNCTION(new, 1, 1);
	DELTA_PUSH_FUNCTION(pack, 1, DELTA_MAX_ARGS);
	
	// string
	DELTA_PUSH_FUNCTION(crc32, 1, 1);
	DELTA_PUSH_FUNCTION(levenshtein, 1, 1);
	DELTA_PUSH_FUNCTION(md5_file, 1, 1);
	DELTA_PUSH_FUNCTION(md5, 1, 1);
	DELTA_PUSH_FUNCTION(sha1, 1, 1);
	DELTA_PUSH_FUNCTION(soundex, 1, 1);
	DELTA_PUSH_FUNCTION(strlen, 1, 1);
	DELTA_PUSH_FUNCTION(strtolower, 1, 1);
	DELTA_PUSH_FUNCTION(strtoupper, 1, 1);
	
	DELTA_PUSH_FUNCTION(substr, 2, 3);
	DELTA_PUSH_FUNCTION(strpos, 2, 2);
	DELTA_PUSH_FUNCTION(ord, 1, 1);
	DELTA_PUSH_FUNCTION(chr, 1, 1);
	DELTA_PUSH_FUNCTION(ltrim, 1, 1);
	DELTA_PUSH_FUNCTION(rtrim, 1, 1);
	DELTA_PUSH_FUNCTION(trim, 1, 1);
	
	// variables
	DELTA_PUSH_FUNCTION(is_array, 1, 1);
	DELTA_PUSH_FUNCTION(echo, 1, DELTA_MAX_ARGS);
	DELTA_PUSH_FUNCTION(print, 0, DELTA_MAX_ARGS);
	DELTA_PUSH_FUNCTION(println, 0, DELTA_MAX_ARGS);
	
	DELTA_END_MODULE_FUNCTIONS;
}


struct DeltaDefine* module_defines(int *count)
{
	DELTA_PREPARE_MODULE_DEFINES(20);
	
	// math constants
	DELTA_PUSH_DEFINE(M_PI, "3.14159265358979323846");
	DELTA_PUSH_DEFINE(M_E, "2.7182818284590452354");
	DELTA_PUSH_DEFINE(M_LOG2E, "1.4426950408889634074");
	DELTA_PUSH_DEFINE(M_LOG10E, "0.43429448190325182765");
	DELTA_PUSH_DEFINE(M_LN2, "0.69314718055994530942");
	DELTA_PUSH_DEFINE(M_LN10, "2.30258509299404568402");
	DELTA_PUSH_DEFINE(M_PI_2, "1.57079632679489661923");
	DELTA_PUSH_DEFINE(M_PI_4, "0.78539816339744830962");
	DELTA_PUSH_DEFINE(M_1_PI, "0.31830988618379067154");
	DELTA_PUSH_DEFINE(M_2_PI, "0.63661977236758134308");
	
	DELTA_PUSH_DEFINE(M_SQRTPI, "1.77245385090551602729");
	DELTA_PUSH_DEFINE(M_2_SQRTPI, "1.12837916709551257390");
	DELTA_PUSH_DEFINE(M_SQRT2, "1.41421356237309504880");
	DELTA_PUSH_DEFINE(M_SQRT3, "1.73205080756887729352");
	DELTA_PUSH_DEFINE(M_SQRT1_2, "0.70710678118654752440");
	DELTA_PUSH_DEFINE(M_LNPI, "1.14472988584940017414");
	DELTA_PUSH_DEFINE(M_EULER, "0.57721566490153286061");
	DELTA_PUSH_DEFINE(INF, "1e2000");
	
	DELTA_END_MODULE_DEFINES;
}


struct DeltaFunctionAlias* module_aliases(int *count)
{
	DELTA_PREPARE_MODULE_FUNCTION_ALIASES(10);
	
	DELTA_PUSH_FUNCTION_ALIAS(native.length, 1, 1, strlen);
	
	DELTA_END_MODULE_FUNCTION_ALIASES;
}

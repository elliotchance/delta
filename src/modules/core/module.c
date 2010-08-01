/*
 *  Delta programming language
 */

#include "module.h"


struct DeltaModuleFunction* module_functions(int *count)
{
	DELTA_PREPARE_MODULE_FUNCTIONS(150);
	
	// array
	DELTA_PUSH_FUNCTION(array, 0, DELTA_MAX_ARGS);
	DELTA_PUSH_FUNCTION(array_keys, 1, 1);
	DELTA_PUSH_FUNCTION(array_push, 3, 3);
	DELTA_PUSH_FUNCTION(array_sum, 0, DELTA_MAX_ARGS);
	DELTA_PUSH_FUNCTION(array_values, 1, 1);
	DELTA_PUSH_FUNCTION(count, 1, 1);
	
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
	
	// misc
	DELTA_PUSH_FUNCTION(exit, 1, 1);
	
	// string
	DELTA_PUSH_FUNCTION(crc32, 1, 1);
	DELTA_PUSH_FUNCTION(levenshtein, 1, 1);
	DELTA_PUSH_FUNCTION(md5_file, 1, 1);
	DELTA_PUSH_FUNCTION(md5, 1, 1);
	DELTA_PUSH_FUNCTION(sha1, 1, 1);
	DELTA_PUSH_FUNCTION(soundex, 1, 1);
	DELTA_PUSH_FUNCTION(strlen, 1, 1);
	DELTA_PUSH_FUNCTION(substr, 2, 3);
	DELTA_PUSH_FUNCTION(strpos, 2, 2);
	
	// variables
	DELTA_PUSH_FUNCTION(is_array, 1, 1);
	DELTA_PUSH_FUNCTION(echo, 1, DELTA_MAX_ARGS);
	DELTA_PUSH_FUNCTION(print, 0, DELTA_MAX_ARGS);
	DELTA_PUSH_FUNCTION(println, 0, DELTA_MAX_ARGS);
	
	DELTA_END_MODULE_FUNCTIONS;
}

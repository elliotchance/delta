/*
 *  Delta programming language
 */

#include "modules.h"

// module includes
#include "modules/array/module.h"
#include "modules/ctype/module.h"
#include "modules/date/module.h"
#include "modules/file/module.h"
#include "modules/io/module.h"
#include "modules/math/module.h"
#include "modules/string/module.h"


int delta_vm_push_function(struct DeltaFunction* f);
struct DeltaFunction*   new_DeltaFunction(char *name,
										  void (*function_ptr)(struct DeltaInstruction *d),
										  int min_args, int max_args);


void delta_load_modules()
{
	// array
	delta_vm_push_function(new_DeltaFunction("array",         FUNC(array), 0, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("array_push",    FUNC(array_push), 2, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("count",         FUNC(count), 1, 1));
	
	// date
	delta_vm_push_function(new_DeltaFunction("time",          FUNC(time), 0, 0));
	
	// file
	delta_vm_push_function(new_DeltaFunction("fopen",         FUNC(fopen), 2, 2));
	delta_vm_push_function(new_DeltaFunction("fwrite",        FUNC(fwrite), 2, 2));
	delta_vm_push_function(new_DeltaFunction("fclose",        FUNC(fclose), 1, 1));
	
	// io
	delta_vm_push_function(new_DeltaFunction("print",         FUNC(print), 1, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("println",       FUNC(println), 1, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("echo",          FUNC(echo), 1, DELTA_MAX_ARGS));
	
	// math
	delta_vm_push_function(new_DeltaFunction("abs",           FUNC(abs), 1, 1));
	delta_vm_push_function(new_DeltaFunction("acos",          FUNC(acos), 1, 1));
	delta_vm_push_function(new_DeltaFunction("acosh",         FUNC(acosh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("asin",          FUNC(asin), 1, 1));
	delta_vm_push_function(new_DeltaFunction("asinh",         FUNC(asinh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("atan",          FUNC(atan), 1, 1));
	delta_vm_push_function(new_DeltaFunction("atan2",         FUNC(atan2), 2, 2));
	delta_vm_push_function(new_DeltaFunction("atanh",         FUNC(atanh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("cos",           FUNC(cos), 1, 1));
	delta_vm_push_function(new_DeltaFunction("sin",           FUNC(sin), 1, 1));
	delta_vm_push_function(new_DeltaFunction("sqrt",          FUNC(sqrt), 1, 1));
	delta_vm_push_function(new_DeltaFunction("tan",           FUNC(tan), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ceil",          FUNC(ceil), 1, 1));
	delta_vm_push_function(new_DeltaFunction("floor",         FUNC(floor), 1, 1));
	delta_vm_push_function(new_DeltaFunction("exp",           FUNC(exp), 1, 1));
	delta_vm_push_function(new_DeltaFunction("log",           FUNC(log), 1, 1));
	delta_vm_push_function(new_DeltaFunction("pi",            FUNC(pi), 0, 0));
	delta_vm_push_function(new_DeltaFunction("log10",         FUNC(log10), 1, 1));
	delta_vm_push_function(new_DeltaFunction("pow",           FUNC(pow), 2, 2));
	delta_vm_push_function(new_DeltaFunction("hypot",         FUNC(hypot), 2, 2));
	delta_vm_push_function(new_DeltaFunction("deg2rad",       FUNC(deg2rad), 1, 1));
	delta_vm_push_function(new_DeltaFunction("rad2deg",       FUNC(rad2deg), 1, 1));
	delta_vm_push_function(new_DeltaFunction("srand",         FUNC(srand), 1, 1));
	delta_vm_push_function(new_DeltaFunction("rand",          FUNC(rand), 0, 0));
	delta_vm_push_function(new_DeltaFunction("cosh",          FUNC(cosh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("sinh",          FUNC(sinh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("tanh",          FUNC(tanh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("getrandmax",    FUNC(getrandmax), 0, 0));
	delta_vm_push_function(new_DeltaFunction("min",           FUNC(min), 1, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("max",           FUNC(max), 1, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("mt_getrandmax", FUNC(getrandmax), 0, 0));
	delta_vm_push_function(new_DeltaFunction("mt_srand",      FUNC(mt_srand), 0, 1));
	delta_vm_push_function(new_DeltaFunction("mt_rand",       FUNC(mt_rand), 0, 0));
	delta_vm_push_function(new_DeltaFunction("expm1",         FUNC(expm1), 1, 1));
	delta_vm_push_function(new_DeltaFunction("log1p",         FUNC(log1p), 1, 1));
	delta_vm_push_function(new_DeltaFunction("fmod",          FUNC(fmod), 2, 2));
	delta_vm_push_function(new_DeltaFunction("lcg_value",     FUNC(lcg_value), 0, 0));
	delta_vm_push_function(new_DeltaFunction("base_convert",  FUNC(base_convert), 3, 3));
	delta_vm_push_function(new_DeltaFunction("bindec",        FUNC(bindec), 1, 1));
	delta_vm_push_function(new_DeltaFunction("decbin",        FUNC(decbin), 1, 1));
	delta_vm_push_function(new_DeltaFunction("dechex",        FUNC(dechex), 1, 1));
	delta_vm_push_function(new_DeltaFunction("decoct",        FUNC(decoct), 1, 1));
	delta_vm_push_function(new_DeltaFunction("hexdec",        FUNC(hexdec), 1, 1));
	delta_vm_push_function(new_DeltaFunction("octdec",        FUNC(octdec), 1, 1));
	
	// string
	delta_vm_push_function(new_DeltaFunction("strlen",        FUNC(strlen), 1, 1));
	delta_vm_push_function(new_DeltaFunction("substr",        FUNC(substr), 2, 3));
	
	// vars
	delta_vm_push_function(new_DeltaFunction("ctype_alnum",   FUNC(ctype_alnum), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_alpha",   FUNC(ctype_alpha), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_cntrl",   FUNC(ctype_cntrl), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_digit",   FUNC(ctype_digit), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_graph",   FUNC(ctype_graph), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_lower",   FUNC(ctype_lower), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_print",   FUNC(ctype_print), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_punct",   FUNC(ctype_punct), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_space",   FUNC(ctype_space), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_upper",   FUNC(ctype_upper), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_xdigit",   FUNC(ctype_xdigit), 1, 1));
}

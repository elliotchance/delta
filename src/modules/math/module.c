/*
 *  Delta programming language
 */

#include "module.h"


// prototypes here so we dont have to include more files
int delta_vm_push_function(struct DeltaFunction* f);
struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args);


void delta_load_module_math()
{
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
}

/*
 *  Delta programming language
 */

#include "structs.h"
#include "ins.h"


#define delta_enumerate_array(__array, __element, __element_id) \
	for(__element_id = 0; i < __array->elements; ++__element_id, __element = __element->next)


DeltaFunction* new_DeltaFunction(char *name, void (*function_ptr)(struct DeltaInstruction *d),
								 int min_args, int max_args);

// array
delta_function(array);
delta_function(array_push);
delta_function(count);

// date
delta_function(time);

// file
delta_function(fopen);
delta_function(fwrite);
delta_function(fclose);

// io
delta_function(print);
delta_function(println);
delta_function(echo);

// math
delta_function(abs);
delta_function(acos);
delta_function(acosh);
delta_function(asin);
delta_function(asinh);
delta_function(atan);
delta_function(atan2);
delta_function(atanh);
delta_function(cos);
delta_function(sin);
delta_function(sqrt);
delta_function(tan);
delta_function(ceil);
delta_function(floor);
delta_function(exp);
delta_function(log);
delta_function(pi);
delta_function(log10);
delta_function(pow);
delta_function(hypot);
delta_function(deg2rad);
delta_function(rad2deg);
delta_function(srand);
delta_function(rand);
delta_function(sinh);
delta_function(cosh);
delta_function(tanh);
delta_function(getrandmax);
delta_function(min);
delta_function(max);
delta_function(mt_getrandmax);
delta_function(mt_srand);
delta_function(mt_rand);
delta_function(expm1);
delta_function(log1p);
delta_function(fmod);
delta_function(lcg_value);
delta_function(base_convert);
delta_function(bindec);
delta_function(decbin);
delta_function(dechex);
delta_function(decoct);
delta_function(hexdec);
delta_function(octdec);

// string
delta_function(strlen);
delta_function(substr);

// vars
delta_function(ctype_alnum);
delta_function(ctype_alpha);
delta_function(ctype_cntrl);
delta_function(ctype_digit);
delta_function(ctype_graph);
delta_function(ctype_lower);
delta_function(ctype_print);
delta_function(ctype_punct);
delta_function(ctype_space);
delta_function(ctype_upper);
delta_function(ctype_xdigit);

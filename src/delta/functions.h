/*
 *  Delta programming language
 */

#include "structs.h"
#include "ins.h"


DeltaFunction* new_DeltaFunction(char *name, void (*function_ptr)(struct DeltaInstruction *d),
								 int min_args, int max_args);

// array
delta_function(array);
delta_function(array_push);
delta_function(count);

// io
delta_function(print);
delta_function(println);

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

// string
delta_function(strlen);
delta_function(substr);

/*
 *  Delta programming language
 */

#include "structs.h"
#include "ins.h"


DeltaFunction* new_DeltaFunction(char *name, void (*function_ptr)(struct DeltaInstruction *d),
								 int min_args, int max_args);

// functions
delta_function(abs);
delta_function(acos);
delta_function(array);
delta_function(array_push);
delta_function(asin);
delta_function(atan);
delta_function(cos);
delta_function(print);
delta_function(println);
delta_function(sin);
delta_function(sqrt);
delta_function(strlen);
delta_function(substr);
delta_function(tan);

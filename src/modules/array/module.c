/*
 *  Delta programming language
 */

#include "module.h"


// prototypes here so we dont have to include more files
int delta_vm_push_function(struct DeltaFunction* f);
struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args);


void delta_load_module_array()
{
	delta_vm_push_function(new_DeltaFunction("array",      FUNC(array), 0, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("array_push", FUNC(array_push), 2, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("count",      FUNC(count), 1, 1));
}

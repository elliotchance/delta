/*
 *  Delta programming language
 */

#include "module.h"


// prototypes here so we dont have to include more files
int delta_vm_push_function(struct DeltaFunction* f);
struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args);


void delta_load_module_io()
{
	delta_vm_push_function(new_DeltaFunction("print",   FUNC(print), 1, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("println", FUNC(println), 1, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("echo",    FUNC(echo), 1, DELTA_MAX_ARGS));
}

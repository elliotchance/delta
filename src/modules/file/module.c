/*
 *  Delta programming language
 */

#include "module.h"


// prototypes here so we dont have to include more files
int delta_vm_push_function(struct DeltaFunction* f);
struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args);


void delta_load_module_file()
{
	delta_vm_push_function(new_DeltaFunction("fopen",  FUNC(fopen), 2, 2));
	delta_vm_push_function(new_DeltaFunction("fwrite", FUNC(fwrite), 2, 2));
	delta_vm_push_function(new_DeltaFunction("fclose", FUNC(fclose), 1, 1));
}

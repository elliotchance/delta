/*
 *  Delta programming language
 */

#include "module.h"


// prototypes here so we dont have to include more files
int delta_vm_push_function(struct DeltaFunction* f);
struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args);


void delta_load_module_ctype()
{
	delta_vm_push_function(new_DeltaFunction("ctype_alnum",  FUNC(ctype_alnum), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_alpha",  FUNC(ctype_alpha), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_cntrl",  FUNC(ctype_cntrl), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_digit",  FUNC(ctype_digit), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_graph",  FUNC(ctype_graph), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_lower",  FUNC(ctype_lower), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_print",  FUNC(ctype_print), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_punct",  FUNC(ctype_punct), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_space",  FUNC(ctype_space), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_upper",  FUNC(ctype_upper), 1, 1));
	delta_vm_push_function(new_DeltaFunction("ctype_xdigit", FUNC(ctype_xdigit), 1, 1));
}

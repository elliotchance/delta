/*
 *  Delta programming language
 */

#include "module.h"


// prototypes here so we dont have to include more files
int delta_vm_push_function(struct DeltaFunction* f);
struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args);


void delta_load_module_errors()
{
	delta_vm_push_define("E_ERROR",        DELTA_ERROR_ERROR);
	delta_vm_push_define("E_WARNING",      DELTA_ERROR_WARNING);
	delta_vm_push_define("E_PARSE",        DELTA_ERROR_PARSE);
	delta_vm_push_define("E_NOTICE",       DELTA_ERROR_NOTICE);
	delta_vm_push_define("E_USER_ERROR",   DELTA_ERROR_USER_ERROR);
	delta_vm_push_define("E_USER_WARNING", DELTA_ERROR_USER_WARNING);
	delta_vm_push_define("E_USER_NOTICE",  DELTA_ERROR_USER_NOTICE);
	
	delta_vm_push_function(new_DeltaFunction("trigger_error", FUNC(trigger_error), 1, 2));
}

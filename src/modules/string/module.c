/*
 *  Delta programming language
 */

#include "module.h"


// prototypes here so we dont have to include more files
int delta_vm_push_function(struct DeltaFunction* f);
struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args);


void delta_load_module_string()
{
	delta_vm_push_function(new_DeltaFunction("addslashes",  FUNC(addslashes), 1, 1));
	delta_vm_push_function(new_DeltaFunction("crc32",       FUNC(crc32), 1, 1));
	delta_vm_push_function(new_DeltaFunction("md5",         FUNC(md5), 1, 1));
	delta_vm_push_function(new_DeltaFunction("md5_file",    FUNC(md5_file), 1, 1));
	delta_vm_push_function(new_DeltaFunction("sha1",        FUNC(sha1), 1, 1));
	delta_vm_push_function(new_DeltaFunction("soundex",     FUNC(soundex), 1, 1));
	delta_vm_push_function(new_DeltaFunction("strlen",      FUNC(strlen), 1, 1));
	delta_vm_push_function(new_DeltaFunction("substr",      FUNC(substr), 2, 3));
	delta_vm_push_function(new_DeltaFunction("levenshtein", FUNC(levenshtein), 1, 1));
}

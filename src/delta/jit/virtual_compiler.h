/*
 *  Delta programming language
 */


#include "delta/structs.h"


delta_virtual_function* delta_compile_virtual(struct DeltaVM *vm, char *function_name);
void                    delta_run_virtual(delta_virtual_function *f);

/*
 *  Delta programming language
 */


#include "delta/structs.h"


struct delta_virtual_function* delta_compile_virtual(struct DeltaVM *vm, char *function_name);
void                           delta_run_virtual(struct delta_virtual_function *f);    

/*
 *  Delta programming language
 */

#ifndef _DELTA_VM_MODULES_H
#define _DELTA_VM_MODULES_H 1


#include "delta/macros.h"


typedef struct DeltaModuleFunction* (*delta_module_ptr)(int*);
typedef void (*delta_module_function)(struct DeltaInstruction *d);
typedef void (*delta_module_function_vm)(struct DeltaVM *vm);


void                  delta_load_modules(struct DeltaVM *vm);
delta_module_function delta_get_module_function(void *module, char *name);
int                   delta_load_module(struct DeltaVM *vm, char *path);


#endif

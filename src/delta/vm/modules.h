/*
 *  Delta programming language
 */

#ifndef _DELTA_VM_MODULES_H
#define _DELTA_VM_MODULES_H 1


#include "delta/macros.h"


typedef struct DeltaModuleFunction* (*delta_module_ptr)(int*);
typedef struct DeltaDefine* (*delta_module_defines_ptr)(int*);
typedef struct DeltaFunctionAlias* (*delta_module_aliases_ptr)(int*);
typedef void (*delta_module_function)(struct DeltaInstruction *d);
typedef void (*delta_module_function_vm)(struct DeltaVM *vm);


extern struct DeltaINI *delta_ini;


void                  delta_load_modules(struct DeltaVM *vm);
delta_module_function delta_get_module_function(void *module, char *name);
int                   delta_load_module(struct DeltaVM *vm, char *path);
void                  delta_load_ini();
void                  delta_load_defines(struct DeltaCompiler *c);
int                   delta_load_module_defines(struct DeltaCompiler *c, char *path);
void                  delta_load_compiler_modules(struct DeltaCompiler *c);
int                   delta_load_compiler_module(struct DeltaCompiler *c, char *path);


#endif

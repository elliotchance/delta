/*
 *  Delta programming language
 */

#ifndef _DELTA_VM_H
#define _DELTA_VM_H


#include "delta/jit/lightning.h"
#include "delta/structs.h"


#define DELTA_MAX_ARGS 1024


extern struct DeltaVM *active_vm;


// TODO: this needs to be automatically worked out at JIT time
#define total_ram 100


struct DeltaVM* delta_vm_init();
void            delta_vm_print_ram(struct DeltaVM *vm);
int             delta_vm_prepare(struct DeltaVM *vm, int function_id, struct DeltaVariable **ram);
void            delta_vm_print_variable(struct DeltaVariable *v);
int             delta_vm_push_function(struct DeltaVM *vm, struct DeltaFunction* f);
int             delta_vm_push_define(struct DeltaCompiler *c, char *name, char *value);
int             delta_vm_push_runtime_error(char *msg, int error_type);
void            delta_release_variable(struct DeltaVariable *v);
struct DeltaVM* delta_get_vm();
void            delta_set_vm(struct DeltaVM *vm);
stack_function  delta_vm_get_function(struct DeltaVM *vm, char *function);


#endif

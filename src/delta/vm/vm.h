/*
 *  Delta programming language
 */

#ifndef _DELTA_VM_H
#define _DELTA_VM_H


#include "lightning.h"
#include "structs.h"


#define DELTA_MAX_ARGS 1024


extern struct DeltaVariable **ram;
extern int stack_pos;
extern long start;
extern struct DeltaFunction **delta_functions;
extern int alloc_delta_functions;
extern int total_delta_functions;
extern struct DeltaDefine *delta_defines;
extern int alloc_delta_defines;
extern int total_delta_defines;


int  delta_vm_init(struct DeltaCompiler *c);
void delta_vm_print_ram(struct DeltaCompiler *c);
int  delta_vm_prepare(struct DeltaCompiler *c);
void delta_vm_print_variable(struct DeltaVariable *v);
int  delta_vm_push_function(struct DeltaFunction* f);
int  delta_vm_push_define(char *name, char *value);


#endif

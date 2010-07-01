/*
 *  vm.h
 *  jit
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#ifndef _DELTA_VM_H
#define _DELTA_VM_H

#include "lightning.h"
#include "structs.h"

#define DELTA_MAX_ARGS 1024


extern struct DeltaVariable **ram;
extern int stack_pos;
extern long start;
extern DeltaFunction **delta_functions;
extern int alloc_delta_functions;
extern int total_delta_functions;


int  delta_vm_init(DeltaCompiler *c);
void delta_vm_print_ram(DeltaCompiler *c);
int  delta_vm_prepare(DeltaCompiler *c);
void delta_vm_print_variable(struct DeltaVariable *v);
int  delta_vm_push_function(DeltaFunction* f);


#endif

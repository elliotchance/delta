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


extern DeltaVariable *ram;
extern int stack_pos;
extern long start;


int  delta_vm_init(DeltaCompiler *c);
void delta_vm_print_ram(DeltaCompiler *c);
int  delta_vm_prepare(DeltaCompiler *c);


#endif

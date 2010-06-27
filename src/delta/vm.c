/*
 *  vm.c
 *  jit
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#include "vm.h"

DeltaVariable *ram;
#define total_ram 30
int stack_pos = 0;
long start;


void delta_vm_print_ram(DeltaCompiler *c)
{
	int i;
	for(i = 0; i < total_ram; ++i) {
		if(ram[i].type == DELTA_TYPE_NULL)
			printf("ram[%d] = (null)\n", i);
		else if(ram[i].type == DELTA_TYPE_NUMBER)
			printf("ram[%d] = %g\n", i, ram[i].value.number);
		else if(ram[i].type == DELTA_TYPE_STRING)
			printf("ram[%d] = '%s'\n", i, ram[i].value.ptr);
	}
}


int delta_vm_init(DeltaCompiler *c)
{	
	// allocate memory
	ram = (DeltaVariable*) malloc(total_ram * sizeof(DeltaVariable));
	
	return DELTA_SUCCESS;
}


int delta_vm_prepare(DeltaCompiler *c)
{
	
	// load constants
	int i;
	for(i = 0; i < c->total_constants; ++i) {
		printf("ram[%d] = %g\n", c->constants[i].ram_location, c->constants[i].value.number);
		ram[c->constants[i].ram_location] = c->constants[i];
	}
	
	return DELTA_SUCCESS;
}

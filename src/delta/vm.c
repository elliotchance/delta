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


void delta_vm_print_variable(DeltaVariable *v)
{
	if(v->type == DELTA_TYPE_NULL)
		printf("(null)");
	else if(v->type == DELTA_TYPE_NUMBER)
		printf("%g", v->value.number);
	else if(v->type == DELTA_TYPE_STRING)
		printf("%s", v->value.ptr);
	else if(v->type == DELTA_TYPE_OBJECT)
		printf("(object)");
	else if(v->type == DELTA_TYPE_ARRAY) {
		// i don't know why, but if this second blank argument isn't in the printf() it will crash.
		printf("Array\n(\n%s", "");
		/*DeltaArrayValue *e = v->value.array.head;
		int i;
		for(i = 0; i < v->value.array.elements; ++i)
			printf("\t[%s] => %s\n", e->key, e->value);*/
		printf(")");
	}
	else
		printf("(invalid)");
}


void delta_vm_print_ram(DeltaCompiler *c)
{
	int i;
	for(i = 0; i < total_ram; ++i) {
		printf("ram[%d] = ", i);
		delta_vm_print_variable(&ram[i]);
		printf("\n");
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
		printf("ram[%d] = ", c->constants[i].ram_location);
		delta_vm_print_variable(&c->constants[i]);
		printf("\n");
		
		ram[c->constants[i].ram_location] = c->constants[i];
	}
	
	return DELTA_SUCCESS;
}

/*
 *  vm.c
 *  jit
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#include "vm.h"
#include "bytecode.h"
#include "functions.h"
#include "ins.h"


DeltaVariable **ram;
#define total_ram 30
int stack_pos = 0;
long start;
DeltaFunction **delta_functions;
int alloc_delta_functions;
int total_delta_functions;


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
		printf("Array\n(\n");
		DeltaArrayValue *e = v->value.array.head;
		int i;
		for(i = 0; i < v->value.array.elements; ++i)
			printf("\t[%s] => %s\n", e->key, e->value);
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
		delta_vm_print_variable(ram[i]);
		printf("\n");
	}
}


int delta_vm_push_function(DeltaFunction* f)
{
	delta_functions[total_delta_functions++] = f;
	return DELTA_SUCCESS;
}


int delta_vm_init(DeltaCompiler *c)
{	
	int i;
	
	// prepare built-in functions
	alloc_delta_functions = 20;
	total_delta_functions = 0;
	delta_functions = (DeltaFunction**) calloc(alloc_delta_functions, sizeof(DeltaFunction*));
	
	delta_vm_push_function(new_DeltaFunction("array_push", func(array_push), 2, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("cos",        func(cos), 1, 1));
	delta_vm_push_function(new_DeltaFunction("print",      func(print), 1, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("println",    func(println), 1, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("sin",        func(sin), 1, 1));
	delta_vm_push_function(new_DeltaFunction("sqrt",       func(sqrt), 1, 1));
	delta_vm_push_function(new_DeltaFunction("strlen",     func(substr), 1, 1));
	delta_vm_push_function(new_DeltaFunction("substr",     func(substr), 2, 3));
	delta_vm_push_function(new_DeltaFunction("tan",        func(tan), 1, 1));
	
	// allocate memory
	ram = (DeltaVariable**) malloc(total_ram * sizeof(DeltaVariable*));
	for(i = 0; i < total_ram; ++i)
		ram[i] = (DeltaVariable*) malloc(sizeof(DeltaVariable));
	
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
		
		ram[c->constants[i].ram_location] = &c->constants[i];
	}
	
	return DELTA_SUCCESS;
}

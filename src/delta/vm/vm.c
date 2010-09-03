/*
 *  Delta programming language
 */

#include "delta/vm/vm.h"
#include "delta/compiler/bytecode.h"
#include "delta/jit/ins.h"
#include "delta/jit/jit_compiler.h"
#include "delta/structs/DeltaVM.h"
#include "modules.h"
#include <assert.h>


struct DeltaVM *active_vm = NULL;


void delta_vm_print_variable(struct DeltaVariable *v)
{
	assert(v != NULL);
	
	if(v->type == DELTA_TYPE_NULL)
		printf("(null)");
	else if(v->type == DELTA_TYPE_BOOLEAN)
		printf("%s", (v->value.number ? "1" : ""));
	else if(v->type == DELTA_TYPE_NUMBER) {
		// if its an integer print non scientific notation
		if((unsigned long) v->value.number == v->value.number)
			printf("%lu", (unsigned long) v->value.number);
		else
			printf("%g", v->value.number);
	}
	else if(v->type == DELTA_TYPE_STRING)
		printf("%s", v->value.ptr);
	else if(v->type == DELTA_TYPE_OBJECT)
		printf("(object)");
	else if(v->type == DELTA_TYPE_RESOURCE)
		printf("(resource: %d)", v->value.resource.id);
	else if(v->type == DELTA_TYPE_ARRAY) {
		printf("Array\n(\n");
		struct DeltaArrayValue *e = v->value.array.head;
		int i;
		for(i = 0; i < v->value.array.elements; ++i, e = e->next) {
			printf("\t[%s] => ", e->key);
			delta_vm_print_variable(e->value);
			printf("\n");
		}
		printf(")");
	}
	else
		printf("(invalid)");
}


void delta_vm_print_ram(struct DeltaVM *vm)
{
	int i, j;
	
	for(i = 0; i < vm->total_functions; ++i) {
		printf("{RAM} Function '%s'\n", vm->functions[i].name);
		if(vm->functions[i].jit_ptr == NULL)
			printf("Not compiled\n");
		else {
			for(j = 0; j < vm->functions[i].total_vars; ++j) {
				printf("ram[%d](%d, %d) = ", j, vm->functions[i].ram[j]->type,
					   vm->functions[i].ram[j]->size);
				delta_vm_print_variable(vm->functions[i].ram[j]);
				printf("\n");
			}
		}
	}
}


int delta_vm_push_function(struct DeltaVM *vm, struct DeltaFunction* f)
{
	vm->delta_functions[vm->total_delta_functions++] = f;
	return DELTA_SUCCESS;
}


int delta_vm_push_define(struct DeltaCompiler *c, char *name, char *value)
{
	c->delta_defines[c->total_delta_defines].name = name;
	c->delta_defines[c->total_delta_defines].value = value;
	++c->total_delta_defines;
	return DELTA_SUCCESS;
}


struct DeltaVM* delta_vm_init()
{	
	struct DeltaVM *vm = new_DeltaVM();
	
	// prepare built-in functions
	vm->alloc_delta_functions = 200;
	vm->total_delta_functions = 0;
	vm->delta_functions = (struct DeltaFunction**)
		calloc(vm->alloc_delta_functions, sizeof(struct DeltaFunction*));
	
	return vm;
}


int delta_vm_prepare(struct DeltaVM *vm, int function_id, struct DeltaVariable **ram)
{
	// load constants
	int i;
	for(i = 0; i < vm->functions[function_id].total_constants; ++i) {
		DELTA_COPY_VARIABLE(ram[vm->functions[function_id].constants[i].ram_location],
							(&vm->functions[function_id].constants[i]));
	}
	
	return DELTA_SUCCESS;
}


int delta_vm_push_runtime_error(char *msg, int error_type)
{
	// first make sure the error_type is valid
	int r = DELTA_TRUE;
	if(error_type < 1 || error_type > 1024)
		r = DELTA_FALSE;
	
	// immeditaly print the message
	if(error_type == atoi(DELTA_ERROR_ERROR))
		printf("ERROR");
	else if(error_type == atoi(DELTA_ERROR_WARNING))
		printf("WARNING");
	else if(error_type == atoi(DELTA_ERROR_PARSE))
		printf("PARSE ERROR");
	else if(error_type == atoi(DELTA_ERROR_NOTICE))
		printf("NOTICE");
	else if(error_type == atoi(DELTA_ERROR_USER_ERROR))
		printf("USER ERROR");
	else if(error_type == atoi(DELTA_ERROR_USER_WARNING))
		printf("USER WARNING");
	else if(error_type == atoi(DELTA_ERROR_USER_NOTICE))
		printf("USER NOTICE");
	else
		printf("UNKNOWN ERROR");
	
	printf(": %s\n", msg);
	
	// the error message is always a copy so remember to freeit when we're finished
	free(msg);
	
	return r;
}


void delta_release_variable(struct DeltaVariable *v)
{
	if(v == NULL)
		return;
	
	free(v->name);
	free(v->value.ptr);
	free(v->value.resource.ptr);
	// TODO: issue #16: free array elements
	
	// free self
	free(v);
}


struct DeltaVM* delta_get_vm()
{
	return active_vm;
}


void delta_set_vm(struct DeltaVM *vm)
{
	active_vm = vm;
}


int delta_vm_function_exists()
{
	return DELTA_YES;
}


stack_function delta_vm_get_function(struct DeltaVM *vm, char *function)
{
	if(vm == NULL)
		return NULL;
	
	int i;
	for(i = 0; i < vm->total_functions; ++i) {
		if(!strcmp(vm->functions[i].name, function))
			return delta_compile_jit(vm, function);
	}
	
	// function was not found
	return NULL;
}

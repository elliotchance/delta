/*
 *  Delta programming language
 */

#include "vm.h"
#include "bytecode.h"
#include "functions.h"
#include "ins.h"


struct DeltaVariable **ram;
#define total_ram 100
int stack_pos = 0;
long start;
DeltaFunction **delta_functions;
int alloc_delta_functions;
int total_delta_functions;


void delta_vm_print_variable(struct DeltaVariable *v)
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


void delta_vm_print_ram(DeltaCompiler *c)
{
	int i;
	for(i = 0; i < total_ram; ++i) {
		if(ram[i]->type != DELTA_TYPE_NULL) {
			printf("ram[%d] = ", i);
			delta_vm_print_variable(ram[i]);
			printf("\n");
		}
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
	alloc_delta_functions = 200;
	total_delta_functions = 0;
	delta_functions = (DeltaFunction**) calloc(alloc_delta_functions, sizeof(DeltaFunction*));
	
	// array
	delta_vm_push_function(new_DeltaFunction("array",      func(array), 0, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("array_push", func(array_push), 2, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("count",      func(count), 1, 1));
	
	// io
	delta_vm_push_function(new_DeltaFunction("print",      func(print), 1, DELTA_MAX_ARGS));
	delta_vm_push_function(new_DeltaFunction("println",    func(println), 1, DELTA_MAX_ARGS));
	
	// math
	delta_vm_push_function(new_DeltaFunction("abs",        func(abs), 1, 1));
	delta_vm_push_function(new_DeltaFunction("acos",       func(acos), 1, 1));
	delta_vm_push_function(new_DeltaFunction("acosh",      func(acosh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("asin",       func(asin), 1, 1));
	delta_vm_push_function(new_DeltaFunction("asinh",      func(asinh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("atan",       func(atan), 1, 1));
	delta_vm_push_function(new_DeltaFunction("atan2",      func(atan2), 2, 2));
	delta_vm_push_function(new_DeltaFunction("atanh",      func(atanh), 1, 1));
	delta_vm_push_function(new_DeltaFunction("cos",        func(cos), 1, 1));
	delta_vm_push_function(new_DeltaFunction("sin",        func(sin), 1, 1));
	delta_vm_push_function(new_DeltaFunction("sqrt",       func(sqrt), 1, 1));
	delta_vm_push_function(new_DeltaFunction("tan",        func(tan), 1, 1));
	
	// string
	delta_vm_push_function(new_DeltaFunction("strlen",     func(strlen), 1, 1));
	delta_vm_push_function(new_DeltaFunction("substr",     func(substr), 2, 3));
	
	// allocate memory
	ram = (struct DeltaVariable**) malloc(total_ram * sizeof(struct DeltaVariable*));
	for(i = 0; i < total_ram; ++i)
		ram[i] = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable));
	
	return DELTA_SUCCESS;
}


int delta_vm_prepare(DeltaCompiler *c)
{
	// load constants
	int i;
	for(i = 0; i < c->total_constants; ++i) {
		ram[c->constants[i].ram_location] = &c->constants[i];
		
		/*printf("ram[%d] = ", c->constants[i].ram_location);
		delta_vm_print_variable(ram[c->constants[i].ram_location]);
		printf("\n");*/
	}
	
	return DELTA_SUCCESS;
}

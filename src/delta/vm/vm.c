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


const char *delta_vartype_as_string[] = {
	"NULL", "BOOLEAN", "NUMBER", "STRING", "ARRAY", "RESOURCE", "OBJECT", "FUNCTION", "UNKNOWN"
};


void delta_vm_print_ram(struct DeltaVM *vm)
{
	assert(vm != NULL);
	int i, j;
	
	for(i = 0; i < vm->total_functions; ++i) {
		printf("{RAM} Function '%s'\n", vm->functions[i].name);
		if(vm->functions[i].jit_ptr == NULL)
			printf("Not compiled\n");
		else {
			for(j = 0; j < vm->functions[i].total_ram; ++j) {
				assert(vm->functions[i].ram[j] != NULL);
				DeltaVariableType type = vm->functions[i].ram[j]->type;
				printf("ram[%d](%s) = ", j, delta_vartype_as_string[type]);
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
		int ram_location = vm->functions[function_id].constants[i].ram_location;
		struct DeltaVariable *arg1 = ram[ram_location];
		struct DeltaVariable *arg2 = &vm->functions[function_id].constants[i];
		assert(arg1 != NULL);
		assert(arg2 != NULL);
		DELTA_COPY_VARIABLE(arg1, arg2);
	}
	
	return DELTA_SUCCESS;
}


/**
 * @brief Throw a runtime error.
 *
 * @see DELTA_TRIGGER_ERRROR()
 */
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


/**
 * @brief Release all the resources of a delta variable.
 * 
 * It is safe to pass a NULL pointer to this function.
 */
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


/**
 * @brief Get the active virtual machine.
 *
 * @see delta_set_vm()
 */
struct DeltaVM* delta_get_vm()
{
	return active_vm;
}


/**
 * @brief Set the active virtual machine.
 *
 * This is used as a communication mechanism for modules that need to interact with the VM that is
 * controlling the executing script.
 *
 * @see delta_get_vm()
 */
void delta_set_vm(struct DeltaVM *vm)
{
	active_vm = vm;
}


/**
 * @brief Check if a function exists.
 *
 * If the function is found to exist but is not JIT compiled it will remain uncompiled.
 *
 * @return DELTA_YES or DELTA_NO.
 */
int delta_vm_function_exists(struct DeltaVM *vm, char *function)
{
	if(vm == NULL)
		return DELTA_NO;
	
	int i;
	
	// first search the user functions
	for(i = 0; i < vm->total_functions; ++i) {
		if(!strcmp(vm->functions[i].name, function))
			return DELTA_YES;
	}
	
	// now search the modules
	for(i = 0; i < vm->total_delta_functions; ++i) {
		if(!strcmp(vm->delta_functions[i]->name, function))
			return DELTA_YES;
	}
	
	// function was not found
	return DELTA_NO;
}


/**
 * @brief Get the JIT handle of a function.
 *
 * @return If the function does not exist it will return NULL. If it does exist but is not JIT
 *         compiled it will be compiled before returning - so as long as this function does not
 *         return NULL you will be able to immediatly use the JIT handle.
 */
delta_jit_function delta_vm_get_function(struct DeltaVM *vm, char *function)
{
	if(vm == NULL)
		return NULL;
	
	int i;
	
	// first search the user functions
	for(i = 0; i < vm->total_functions; ++i) {
		if(!strcmp(vm->functions[i].name, function))
			return delta_compile_jit(vm, function);
	}
	
	// now search the modules
	for(i = 0; i < vm->total_delta_functions; ++i) {
		if(!strcmp(vm->delta_functions[i]->name, function))
			return vm->delta_functions[i]->function_ptr;
	}
	
	// function was not found
	return NULL;
}


int delta_calculate_total_ram(struct DeltaCompiledFunction *f)
{
	assert(f != NULL);
	int r = 0, i, j;
	
	for(i = 0; i < f->total_ins; ++i) {
		for(j = 0; j < f->ins[i].args; ++j) {
			if(f->ins[i].arg[j] > r)
				r = f->ins[i].arg[j];
		}
	}
	
	for(i = 0; i < f->total_constants; ++i) {
		if(f->constants[i].ram_location > r)
			r = f->constants[i].ram_location;
	}
	
	//printf("ram required = %d\n", r + 1);
	return r + 1;
}


int delta_calculate_total_static_ram(struct DeltaCompiledFunction *f)
{
	assert(f != NULL);
	int r = 0, i, j;
	
	for(i = 0; i < f->total_ins; ++i) {
		for(j = 0; j < f->ins[i].args; ++j) {
			if(f->ins[i].arg[j] < r)
				r = f->ins[i].arg[j];
		}
	}
	
	for(i = 0; i < f->total_constants; ++i) {
		if(f->constants[i].ram_location < r)
			r = f->constants[i].ram_location;
	}
	
	//printf("static ram required = %d\n", -r + 1);
	return -r + 1;
}

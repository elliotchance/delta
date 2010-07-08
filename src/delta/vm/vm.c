/*
 *  Delta programming language
 */

#include "vm.h"
#include "bytecode.h"
#include "ins.h"
#include "modules.h"


struct DeltaVariable **ram;
#define total_ram 100
int stack_pos = 0;
long start;
struct DeltaFunction **delta_functions;
int alloc_delta_functions;
int total_delta_functions;
struct DeltaDefine *delta_defines;
int alloc_delta_defines;
int total_delta_defines;


void delta_vm_print_variable(struct DeltaVariable *v)
{
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


void delta_vm_print_ram(struct DeltaCompiler *c)
{
	int i;
	for(i = 0; i < total_ram; ++i) {
		if(ram[i]->type != DELTA_TYPE_NULL) {
			printf("ram[%d](%d) = ", i, ram[i]->type);
			delta_vm_print_variable(ram[i]);
			printf("\n");
		}
	}
}


int delta_vm_push_function(struct DeltaFunction* f)
{
	delta_functions[total_delta_functions++] = f;
	return DELTA_SUCCESS;
}


int delta_vm_push_define(char *name, char *value)
{
	delta_defines[total_delta_defines].name = name;
	delta_defines[total_delta_defines].value = value;
	++total_delta_defines;
	return DELTA_SUCCESS;
}


int delta_vm_init(struct DeltaCompiler *c)
{	
	int i;
	
	// prepare built-in predefined constants
	alloc_delta_defines = 200;
	total_delta_defines = 0;
	delta_defines = (struct DeltaDefine*) calloc(alloc_delta_defines, sizeof(struct DeltaDefine));
	
	// prepare built-in functions
	alloc_delta_functions = 200;
	total_delta_functions = 0;
	delta_functions = (struct DeltaFunction**) calloc(alloc_delta_functions, sizeof(struct DeltaFunction*));
	
	delta_load_modules();
	
	// allocate memory
	ram = (struct DeltaVariable**) malloc(total_ram * sizeof(struct DeltaVariable*));
	for(i = 0; i < total_ram; ++i)
		ram[i] = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable));
	
	return DELTA_SUCCESS;
}


int delta_vm_prepare(struct DeltaCompiler *c)
{
	// load constants
	int i;
	for(i = 0; i < c->total_constants; ++i) {
		ram[c->constants[i].ram_location]->type = c->constants[i].type;
		ram[c->constants[i].ram_location]->value.number = c->constants[i].value.number;
		ram[c->constants[i].ram_location]->value.ptr = c->constants[i].value.ptr;
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


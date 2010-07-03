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
struct DeltaDefine *delta_defines;
int alloc_delta_defines;
int total_delta_defines;


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


int delta_vm_push_define(char *name, char *value)
{
	delta_defines[total_delta_defines].name = name;
	delta_defines[total_delta_defines].value = value;
	++total_delta_defines;
	return DELTA_SUCCESS;
}


int delta_vm_init(DeltaCompiler *c)
{	
	int i;
	
	// prepare built-in predefined constants
	alloc_delta_defines = 200;
	total_delta_defines = 0;
	delta_defines = (struct DeltaDefine*) calloc(alloc_delta_defines, sizeof(struct DeltaDefine));
	
	delta_vm_push_define("M_PI",       "3.14159265358979323846");
	delta_vm_push_define("M_E",        "2.7182818284590452354");
	delta_vm_push_define("M_LOG2E",    "1.4426950408889634074");
	delta_vm_push_define("M_LOG10E",   "0.43429448190325182765");
	delta_vm_push_define("M_LN2",      "0.69314718055994530942");
	delta_vm_push_define("M_LN10",     "2.30258509299404568402");
	delta_vm_push_define("M_PI_2",     "1.57079632679489661923");
	delta_vm_push_define("M_PI_4",     "0.78539816339744830962");
	delta_vm_push_define("M_1_PI",     "0.31830988618379067154");
	delta_vm_push_define("M_2_PI",     "0.63661977236758134308");
	delta_vm_push_define("M_SQRTPI",   "1.77245385090551602729");
	delta_vm_push_define("M_2_SQRTPI", "1.12837916709551257390");
	delta_vm_push_define("M_SQRT2",    "1.41421356237309504880");
	delta_vm_push_define("M_SQRT3",    "1.73205080756887729352");
	delta_vm_push_define("M_SQRT1_2",  "0.70710678118654752440");
	delta_vm_push_define("M_LNPI",     "1.14472988584940017414");
	delta_vm_push_define("M_EULER",    "0.57721566490153286061");
	
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
	delta_vm_push_function(new_DeltaFunction("ceil",       func(ceil), 1, 1));
	delta_vm_push_function(new_DeltaFunction("floor",      func(floor), 1, 1));
	delta_vm_push_function(new_DeltaFunction("exp",        func(exp), 1, 1));
	delta_vm_push_function(new_DeltaFunction("log",        func(log), 1, 1));
	delta_vm_push_function(new_DeltaFunction("pi",         func(pi), 0, 0));
	delta_vm_push_function(new_DeltaFunction("log10",      func(log10), 1, 1));
	delta_vm_push_function(new_DeltaFunction("pow",        func(pow), 2, 2));
	delta_vm_push_function(new_DeltaFunction("hypot",      func(hypot), 2, 2));
	delta_vm_push_function(new_DeltaFunction("deg2rad",    func(deg2rad), 1, 1));
	delta_vm_push_function(new_DeltaFunction("rad2deg",    func(rad2deg), 1, 1));
	delta_vm_push_function(new_DeltaFunction("srand",      func(srand), 1, 1));
	delta_vm_push_function(new_DeltaFunction("rand",       func(rand), 0, 0));
	
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

/*
 *  Delta programming language
 */

#include "compiler.h"
#include "bytecode.h"
#include "delta/vm/vm.h"
#include "strings.h"
#include "errors.h"
#include "delta/structs/DeltaInstruction.h"
#include "delta/structs/DeltaFunction.h"
#include "delta/structs/DeltaCompiler.h"
#include "token.h"
#include "constant.h"
#include "delta/macros.h"
#include "delta/jit/optimiser.h"
#include "delta/compiler/bytecode_writer.h"
#include "delta/compiler/compile_block.h"
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <assert.h>


#define DELTA_MAX_NESTED_FUNCTIONS 16
#define DELTA_MAX_FUNCTION_ARGS 16
#define DELTA_INF 1e2000


int var_temp = 0;
int result_register = 0;
int *arg_count = NULL;
int **arg_ptr = NULL;
int arg_depth = 0;
int label_id = 0;
int subexpression_depth = 0;
int line_number = 1;
char *class_name = NULL;


int delta_is_constructor(char *name)
{
	int len = (strlen(name) - 1) / 2;
	return !strncmp(name, name + len + 1, len);
}


void delta_function_reset()
{
	var_temp = 0;
	result_register = 0;
	arg_depth = 0;
	label_id = 0;
	subexpression_depth = 1;
	arg_count = (int*) calloc(DELTA_MAX_NESTED_FUNCTIONS, sizeof(int));
	arg_ptr = (int**) calloc(DELTA_MAX_NESTED_FUNCTIONS, sizeof(int*));
	
	int i;
	for(i = 0; i < DELTA_MAX_NESTED_FUNCTIONS; ++i)
		arg_ptr[i] = (int*) calloc(DELTA_MAX_FUNCTION_ARGS, sizeof(int));
}


int delta_push_label(struct DeltaCompiler *c, char *name)
{
	// TODO: fixme
	return 0;
}


void delta_die(const char* msg)
{
	printf("%s", msg);
	exit(1);
}


int delta_compile_file(struct DeltaCompiler *c, const char* input_file)
{
	// prepare
	FILE *f = fopen(input_file, "r");
	if(f == NULL)
		delta_die("Could not open input file\n");
	
	// read the contents of the file
	fseek(f, 0, SEEK_END);
	long total_bytes = ftell(f);
	char *whole_file = (char*) malloc(total_bytes);
	fseek(f, 0, SEEK_SET);
	fread(whole_file, total_bytes, 1, f);
	
	// compile as a block
	int i;
	delta_function_reset();
	delta_compile_block(c, "", whole_file, 0, total_bytes);
	
	// the main function is the function that will not have a name
	++c->total_functions;
	for(i = 0; i < c->total_functions; ++i) {
		if(c->functions[i].name == NULL) {
			c->functions[i].name = DELTA_MAIN_FUNCTION;
			c->functions[i].jit_ptr = NULL;
		}
	}
	
	// optimise bytecode for all functions
	for(i = 0; i < c->total_functions; ++i)
		delta_optimise_bytecode(c, i);
	
	// clean up
	fclose(f);
	free(whole_file);
	return DELTA_SUCCESS;
}


void delta_compiler_defaults(struct DeltaCompiler* c)
{
	c->option_virtual_vm = 0;
}


struct DeltaCompiler* delta_compiler_init()
{	
	struct DeltaCompiler *c = new_DeltaCompiler();
	int i;
	
	// prepare built-in predefined constants
	c->alloc_delta_defines = 200;
	c->total_delta_defines = 0;
	c->delta_defines = (struct DeltaDefine*)
		calloc(c->alloc_delta_defines, sizeof(struct DeltaDefine));
	
	// set defaults
	c->errors = 0;
	delta_compiler_defaults(c);
	
	return c;
}


int delta_needs_compile(char *file1, char *file2)
{
	struct stat stat1, stat2;
	stat(file1, &stat1);
	stat(file2, &stat2);
	
	return stat1.st_mtime != stat2.st_mtime;
}

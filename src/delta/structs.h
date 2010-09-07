/*
 *  Delta programming language
 */

#ifndef _DELTA_STRUCTS_H
#define _DELTA_STRUCTS_H


#include "delta/jit/lightning.h"


typedef unsigned char DeltaByteCode;
typedef char DeltaVariableType;


struct DeltaArrayValue;
struct DeltaVariable;


struct DeltaArrayValue
{
	char *key;
	struct DeltaVariable *value;
	struct DeltaArrayValue *next;
};


struct DeltaINI
{
	int module_count;
	char **module_paths;
};


struct DeltaDefine
{
	char *name;
	char *value;
};


struct DeltaFunctionAlias
{
	char *original;
	char *alias;
	int min_args;
	int max_args;
	int is_static;
	int permission;
};


struct DeltaArray
{
	int elements;
	struct DeltaArrayValue *head;
	struct DeltaArrayValue *tail;
};


struct DeltaResource
{
	char* ptr;
	int id;
};


struct DeltaVariable
{
	//! The name for this variable.
	char* name;
	
	//! The data type for this variable.
	DeltaVariableType type;
	
	//! Variable value.
	union
	{
		//! Numerical value.
		double number;
		
		//! String value.
		char *ptr;
		
		//! Array value.
		struct DeltaArray array;
		
		//! Resource value
		struct DeltaResource resource;
	} value;
	
	//! RAM location of this variable.
	int ram_location;
	
	//! To make strings binary safe we have a separate size for ptr.
	int size;
	
	//! Safe variable type.
	DeltaVariableType safe_type;
};


struct DeltaLabel
{
	char* name;
	jit_insn *begin;
};

struct delta_virtual_function;

struct DeltaInstruction
{
	char* func;
	DeltaByteCode bc;
	int args;
	int *arg;
	struct DeltaVariable **varg;
	struct delta_virtual_function *virtual_ptr;
};


typedef void (*delta_jit_function)(struct DeltaInstruction*);


typedef struct {
	delta_jit_function func;
	struct DeltaInstruction ins;
} delta_virtual_instruction;


struct delta_virtual_function {
	int alloc_ins;
	int total_ins;
	delta_virtual_instruction *ins;
};


struct DeltaUncompiledFunction
{
	char *name;
	char *body;
};


struct DeltaCompiledFunction
{
	char *name;
	
	int alloc_ins;
	int total_ins;
	struct DeltaInstruction *ins;
	
	delta_jit_function jit_ptr;
	struct delta_virtual_function *virtual_ptr;
	
	int alloc_vars, total_vars;
	struct DeltaVariable *vars;
	
	int alloc_labels, total_labels;
	struct DeltaLabel *labels;
	
	int alloc_constants, total_constants;
	struct DeltaVariable *constants;
	
	struct DeltaVariable **ram;
};


struct DeltaFunction
{
	char* name;
	delta_jit_function function_ptr;
	int min_args;
	int max_args;
	int is_static;
	int permission;
};


struct DeltaCompiler
{
	int alloc_functions, total_functions;
	struct DeltaCompiledFunction *functions;
	
	struct DeltaDefine *delta_defines;
	int alloc_delta_defines;
	int total_delta_defines;
	
	int errors;
	
	int option_virtual_vm;
};


struct DeltaVM
{
	int alloc_functions;
	int total_functions;
	struct DeltaCompiledFunction *functions;
	
	struct DeltaFunction **delta_functions;
	int alloc_delta_functions;
	int total_delta_functions;
};


struct DeltaModuleFunction
{
	char* name;
	int min_args;
	int max_args;
	int is_static;
	int permission;
};


#endif

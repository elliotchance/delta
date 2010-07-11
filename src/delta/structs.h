/*
 *  Delta programming language
 */

#ifndef _DELTA_STRUCTS_H
#define _DELTA_STRUCTS_H


#include "lightning.h"


typedef unsigned short DeltaByteCode;
typedef char DeltaVariableType;


struct DeltaArrayValue;
struct DeltaVariable;

struct DeltaArrayValue
{
	char *key;
	struct DeltaVariable *value;
	struct DeltaArrayValue *next;
};


struct DeltaDefine
{
	char *name;
	char *value;
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


struct DeltaInstruction
{
	char* func;
	DeltaByteCode bc;
	int args;
	int *arg;
	struct DeltaVariable **varg;
};


typedef void (*stack_function)(struct DeltaInstruction*);


struct DeltaFunction
{
	char* name;
	void (*function_ptr)(struct DeltaInstruction *d);
	int min_args;
	int max_args;
};


struct DeltaCompiler
{
	int alloc_ins, total_ins;
	struct DeltaInstruction *ins;
	
	int alloc_vars, total_vars;
	struct DeltaVariable* vars;
	
	int alloc_labels, total_labels;
	struct DeltaLabel* labels;
	
	int alloc_constants, total_constants;
	struct DeltaVariable* constants;
};


struct DeltaModuleFunction
{
	char* name;
	int min_args;
	int max_args;
};


#endif

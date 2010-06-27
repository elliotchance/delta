/*
 *  structs.h
 *  jit
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#ifndef _DELTA_STRUCTS_H
#define _DELTA_STRUCTS_H

#include "lightning.h"


#define DELTA_SUCCESS 0
#define DELTA_YES     1
#define DELTA_NO      0


typedef unsigned short DeltaByteCode;
typedef char DeltaVariableType;
#define DELTA_TYPE_NULL   0
#define DELTA_TYPE_NUMBER 1
#define DELTA_TYPE_STRING 2
#define DELTA_TYPE_OBJECT 3


typedef struct
{
	char* name;
	DeltaVariableType type;
	union
	{
		double number;
		char *ptr;
	} value;
	int ram_location;
} DeltaVariable;


typedef struct
{
	char* name;
	jit_insn *begin;
} DeltaLabel;


struct DeltaInstruction;
typedef void (*stack_function)(struct DeltaInstruction*);


struct DeltaInstruction
{
	stack_function f;
	DeltaByteCode bc;
	int destination;
	int source1;
	int source2;
};
typedef struct DeltaInstruction DI;


typedef struct
{
	int dummy;
} DeltaFunction;


typedef struct
{
	int total_functions;
	DeltaFunction **f;
} DeltaObject;


typedef struct
{
	//int total_objects;
	//DeltaObject **o;
	
	int alloc_ins, total_ins;
	DI *ins;
	
	int alloc_vars, total_vars;
	DeltaVariable* vars;
	
	int alloc_labels, total_labels;
	DeltaLabel* labels;
	
	int alloc_constants, total_constants;
	DeltaVariable* constants;
} DeltaCompiler;


#endif

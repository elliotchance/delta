/*
 *  Delta programming language
 */

#include "DeltaFunction.h"


struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args)
{
	struct DeltaFunction *f = (struct DeltaFunction*) malloc(sizeof(struct DeltaFunction));
	
	f->name = name;
	f->function_ptr = function_ptr;
	f->min_args = min_args;
	f->max_args = max_args;
	
	return f;
}


void DeltaFunction_push(struct DeltaCompiler* c, struct DeltaInstruction ins)
{
	c->ins[c->total_ins++] = ins;
}

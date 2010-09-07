/*
 *  Delta programming language
 */

#include "DeltaFunction.h"
#include "delta/macros.h"


struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args, int is_static, int permission)
{
	struct DeltaFunction *f = (struct DeltaFunction*) malloc(sizeof(struct DeltaFunction));
	
	f->name = name;
	f->function_ptr = function_ptr;
	f->min_args = min_args;
	f->max_args = max_args;
	f->is_static = is_static;
	f->permission = permission;
	
	return f;
}


void DeltaFunction_push(struct DeltaCompiler* c, int function_id, struct DeltaInstruction ins)
{
	c->functions[function_id].ins[c->functions[function_id].total_ins++] = ins;
}


void free_DeltaFunction(struct DeltaFunction *f)
{
	/*int i;
	for(i = 0; i < f->alloc_ins; ++i)
		free_DeltaInstruction(f->ins[i]);*/
	free(f);
}

/*
 *  Delta programming language
 */

#include "functions.h"


DeltaFunction* new_DeltaFunction(char *name, void (*function_ptr)(struct DeltaInstruction *d),
								 int min_args, int max_args)
{
	DeltaFunction *f = (DeltaFunction*) malloc(sizeof(DeltaFunction));
	
	f->name = name;
	f->function_ptr = function_ptr;
	f->min_args = min_args;
	f->max_args = max_args;
	
	return f;
}

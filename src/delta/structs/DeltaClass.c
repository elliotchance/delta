/*
 *  Delta programming language
 */

#include "DeltaClass.h"

struct DeltaClass* new_DeltaClass()
{
	struct DeltaClass *cl = (struct DeltaClass*) malloc(sizeof(struct DeltaClass));
	
	cl->alloc_member_variables = 10;
	cl->total_member_variables = 0;
	cl->member_variables = (struct DeltaVariable*) calloc(cl->alloc_member_variables,
														  sizeof(struct DeltaClass));
	
	return cl;
}

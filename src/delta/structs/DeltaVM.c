/*
 *  Delta programming language
 */

#include "DeltaVM.h"
#include "DeltaFunction.h"


struct DeltaVM* new_DeltaVM()
{
	struct DeltaVM *vm = (struct DeltaVM*) malloc(sizeof(struct DeltaVM));
	
	return vm;
}


void free_DeltaVM(struct DeltaVM *vm)
{
	int i;
	for(i = 0; i < vm->alloc_delta_functions; ++i)
		free_DeltaFunction(vm->delta_functions[i]);
	free(vm);
}

/*
 *  Delta programming language
 */

#include "DeltaVM.h"


struct DeltaVM* new_DeltaVM()
{
	struct DeltaVM *vm = (struct DeltaVM*) malloc(sizeof(struct DeltaVM));
	
	return vm;
}

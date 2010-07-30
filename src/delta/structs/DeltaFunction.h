/*
 *  Delta programming language
 */

#include "delta/structs.h"


struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args);
void                  DeltaFunction_push(struct DeltaCompiler* c, int function_id,
										 struct DeltaInstruction ins);

/*
 *  Delta programming language
 */

#ifndef _DELTA_JIT_COMPILER_H
#define _DELTA_JIT_COMPILER_H


#include "delta/structs.h"


delta_jit_function delta_compile_jit(struct DeltaVM *c, char *function_name);
void               delta_check_permission(struct DeltaVM *vm, int i, int j,
										  struct DeltaInstruction *instructions);
void               delta_check_static(struct DeltaVM *vm, int i, int j,
									  struct DeltaInstruction *instructions);


#endif

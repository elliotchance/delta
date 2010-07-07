/*
 *  Delta programming language
 */

#ifndef _DELTA_JIT_COMPILER_H
#define _DELTA_JIT_COMPILER_H


#include "structs.h"

stack_function delta_compile_jit(struct DeltaCompiler *c, int start, int end);


#endif

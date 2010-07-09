/*
 *  Delta programming language
 */

#include "delta/structs.h"


void delta_optimise_bytecode(struct DeltaCompiler *c, int at, int end);
int  delta_is_math_bytecode(DeltaByteCode bc);
int  delta_is_compare_bytecode(DeltaByteCode bc);
int  delta_is_temp(struct DeltaCompiler *c, int address);
int  delta_translate_safe_bytecode(int bc);

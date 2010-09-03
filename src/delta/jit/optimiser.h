/*
 *  Delta programming language
 */

#include "delta/structs.h"


void delta_optimise_bytecode(struct DeltaCompiler *c, int function_id);
int  delta_is_math_bytecode(DeltaByteCode bc);
int  delta_is_compare_bytecode(DeltaByteCode bc);
int  delta_is_temp(struct DeltaCompiler *c, int function_id, int address);
int  delta_translate_safe_bytecode(int bc);
void delta_optimiser_addresses(struct DeltaCompiler *c, int function_id, int at, int end);
void delta_optimiser_bytecodes(struct DeltaCompiler *c, int function_id, int at, int end);
void delta_optimiser_fastmath(struct DeltaCompiler *c, int function_id, int at, int end);

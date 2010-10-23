/*
 *  Delta programming language
 */

#include "delta/structs.h"


int delta_compile_block(struct DeltaCompiler *c, char *identifier, char *block, int at, int end,
						int function_id);

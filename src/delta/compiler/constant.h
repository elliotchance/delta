/*
 *  Delta programming language
 */

#include "delta/structs.h"


int   delta_push_constant(struct DeltaCompiler *c, int function_id, char *token, int escape);
char* delta_replace_constant(struct DeltaCompiler *c, char *token);
int   delta_push_number_constant(struct DeltaCompiler *c, int function_id, double value);
int   delta_push_string_constant(struct DeltaCompiler *c, int function_id, char *token, int escape);

/*
 *  Delta programming language
 */

#include "delta/structs.h"


int   delta_push_constant(struct DeltaCompiler *c, char *token);
char* delta_replace_constant(char *token);
int   delta_push_number_constant(struct DeltaCompiler *c, double value);

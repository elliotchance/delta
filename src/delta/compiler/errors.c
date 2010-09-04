/*
 *  Delta programming language
 */

#include "errors.h"

void delta_error_push(struct DeltaCompiler *c, int line, char *msg)
{
	++c->errors;
	if(line > 0)
		printf("Error: Line %d: %s\n", line, msg);
	else
		printf("Error: %s\n", msg);
}

/*
 *  Delta programming language
 */

#include "delta/structs.h"


char* delta_read_token(struct DeltaCompiler *c, int function_id, char* line, int* offset);
char* delta_read_inv_token(char* line, int* offset);
void  delta_skip_spaces(char* line, int* offset);
int   delta_is_keyword(char* word);
int   delta_is_declared(struct DeltaCompiler *c, int function_id, char* varname);
int   delta_get_variable_id(struct DeltaCompiler *c, int function_id, char* name);
int   delta_get_operator_order(char* op);
int   delta_is_user_function(struct DeltaCompiler *c, char *name);

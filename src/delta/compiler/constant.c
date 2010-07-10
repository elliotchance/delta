/*
 *  Delta programming language
 */

#include "constant.h"
#include "compiler.h"
#include "strings.h"
#include "delta/macros.h"
#include "delta/vm/vm.h"
#include <string.h>


int delta_push_constant(struct DeltaCompiler *c, char *token)
{
	if(delta_is_number(token))
		return delta_push_number_constant(c, atof(token));
	
	++var_temp;
	c->constants[c->total_constants].type = DELTA_TYPE_STRING;
	c->constants[c->total_constants].value.ptr = token;
	c->constants[c->total_constants].ram_location = var_temp;
	c->constants[c->total_constants].size = strlen(token);
	delta_escape_string(c->constants[c->total_constants].value.ptr,
						c->constants[c->total_constants].size);
	++c->total_constants;
	return var_temp;
}


int delta_push_number_constant(struct DeltaCompiler *c, double value)
{
	++var_temp;
	c->constants[c->total_constants].type = DELTA_TYPE_NUMBER;
	c->constants[c->total_constants].value.number = value;
	c->constants[c->total_constants].ram_location = var_temp;
	++c->total_constants;
	return var_temp;
}


char* delta_replace_constant(char *token)
{
	int i;
	for(i = 0; i < total_delta_defines; ++i) {
		if(!strcmp(token, delta_defines[i].name))
			return delta_copy_string(delta_defines[i].value);
	}
	
	return token;
}

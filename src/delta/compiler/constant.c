/*
 *  Delta programming language
 */

#include "constant.h"
#include "compiler.h"
#include "strings.h"
#include "delta/macros.h"
#include "delta/vm/vm.h"
#include <string.h>


int delta_push_constant(struct DeltaCompiler *c, int function_id, char *token, int escape)
{
	if(delta_is_number(token))
		return delta_push_number_constant(c, function_id, atof(token));
	
	++var_temp;
	c->functions[function_id].constants[c->functions[function_id].total_constants].type = DELTA_TYPE_STRING;
	c->functions[function_id].constants[c->functions[function_id].total_constants].value.ptr = token;
	c->functions[function_id].constants[c->functions[function_id].total_constants].ram_location = var_temp;
	c->functions[function_id].constants[c->functions[function_id].total_constants].size = strlen(token);
	if(escape)
		delta_escape_string(c->functions[function_id].constants[c->functions[function_id].total_constants].value.ptr,
							c->functions[function_id].constants[c->functions[function_id].total_constants].size);
	++c->functions[function_id].total_constants;
	return var_temp;
}


int delta_push_number_constant(struct DeltaCompiler *c, int function_id, double value)
{
	++var_temp;
	c->functions[function_id].constants[c->functions[function_id].total_constants].type = DELTA_TYPE_NUMBER;
	c->functions[function_id].constants[c->functions[function_id].total_constants].value.number = value;
	c->functions[function_id].constants[c->functions[function_id].total_constants].ram_location = var_temp;
	++c->functions[function_id].total_constants;
	return var_temp;
}


char* delta_replace_constant(struct DeltaCompiler *c, char *token)
{
	int i;
	for(i = 0; i < c->total_delta_defines; ++i) {
		if(!strcmp(token, c->delta_defines[i].name))
			return delta_copy_string(c->delta_defines[i].value);
	}
	
	return token;
}

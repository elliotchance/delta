/*
 *  Delta programming language
 */

#include "constant.h"
#include "compiler.h"
#include "strings.h"
#include "delta/macros.h"
#include "delta/vm/vm.h"
#include <string.h>
#include <assert.h>


int delta_push_string_constant(struct DeltaCompiler *c, int function_id, char *token, int escape)
{
	assert(function_id >= 0);
	
	int total = c->functions[function_id].total_constants;
	struct DeltaVariable* v = &c->functions[function_id].constants[total];
	
	++var_temp;
	v->type = DELTA_TYPE_STRING;
	v->value.ptr = token;
	v->ram_location = var_temp;
	v->size = strlen(token);
	
	if(escape)
		delta_escape_string(v->value.ptr, v->size);
	
	++c->functions[function_id].total_constants;
	return var_temp;
}


int delta_push_constant(struct DeltaCompiler *c, int function_id, char *token, int escape)
{
	if(delta_is_number(token))
		return delta_push_number_constant(c, function_id, atof(token));
	return delta_push_string_constant(c, function_id, token, escape);
}


int delta_push_number_constant(struct DeltaCompiler *c, int function_id, double value)
{
	int total = c->functions[function_id].total_constants;
	struct DeltaVariable* v = &c->functions[function_id].constants[total];
	
	++var_temp;
	v->type = DELTA_TYPE_NUMBER;
	v->value.number = value;
	v->ram_location = var_temp;
	
	++c->functions[function_id].total_constants;
	return var_temp;
}


int delta_push_boolean_constant(struct DeltaCompiler *c, int function_id, int value)
{
	int total = c->functions[function_id].total_constants;
	struct DeltaVariable* v = &c->functions[function_id].constants[total];
	
	++var_temp;
	v->type = DELTA_TYPE_BOOLEAN;
	v->value.number = value;
	v->ram_location = var_temp;
	
	++c->functions[function_id].total_constants;
	return var_temp;
}


int delta_push_null_constant(struct DeltaCompiler *c, int function_id)
{
	int total = c->functions[function_id].total_constants;
	struct DeltaVariable* v = &c->functions[function_id].constants[total];
	
	++var_temp;
	v->type = DELTA_TYPE_NULL;
	v->ram_location = var_temp;
	
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

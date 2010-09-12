/*
 *  Delta programming language
 */

#include "token.h"
#include "compiler.h"
#include "bytecode.h"
#include "delta/structs/DeltaInstruction.h"
#include "delta/structs/DeltaFunction.h"
#include "strings.h"
#include "delta/macros.h"
#include "delta/compiler/bytecode_writer.h"
#include "delta/compiler/compile_line_part.h"
#include <string.h>
#include <ctype.h>
#include <assert.h>


char* delta_read_inv_token(char* line, int* offset)
{
	int orig = *offset, len = strlen(line);
	
	for(; *offset < len; ++*offset) {
		if(isalnum(line[*offset]) || isspace(line[*offset]))
			break;
	}
	
	char* r = (char*) malloc(orig + 1);
	strncpy(r, line + orig, *offset - orig);
	return r;
}


void delta_skip_spaces(char* line, int* offset)
{
	int len = strlen(line);
	for(; *offset < len; ++*offset) {
		if(!isspace(line[*offset]))
			return;
	}
}


int delta_is_keyword(char* word)
{
	if(!strcmp(word, "new"))
		return 1;
	if(!strcmp(word, "return"))
		return 1;
	return 0;
}


int delta_is_declared(struct DeltaCompiler *c, int function_id, char* varname)
{
	assert(function_id >= 0);
	//assert(function_id < c->total_functions);
	
	int i;
	for(i = 0; i < c->functions[function_id].total_vars; ++i) {
		if(!strcmp(c->functions[function_id].vars[i].name, varname))
			return 1;
	}
	return 0;
}


int delta_get_variable_id(struct DeltaCompiler *c, int function_id, char* name)
{
	// safety
	if(name == NULL)
		return -1;
	
	// argument by ID
	if(name[0] == '$') {
		int temp = -1;
		sscanf(name, "$%d", &temp);
		int var_dest = ++var_temp;
		
		// compensate for the fact the real position of the argument value is higher
		temp = (temp + 1) * 2;
		
		// add an instruction to copy out the argument
		DELTA_WRITE_BYTECODE(BYTECODE_ARG, "", function_id,
							 new_DeltaInstruction2(NULL, BYTECODE_ARG, var_dest, temp));
		
		return var_dest;
	}
	
	// strip off array element
	char *element = NULL;
	int pos = delta_strpos(name, "[");
	if(pos >= 0) {
		int end = delta_strpos(name, "]");
		element = (char*) malloc(end - pos);
		strncpy(element, name + pos + 1, end - pos - 1);
		name[pos] = 0;
	}
	//printf("looking for '%s' [%s]\n", name, element);
	
	// a register address
	if(name[0] == '#') {
		int temp = -1;
		sscanf(name, "#%d", &temp);
		return temp;
	}
	
	int i, location = -1;
	for(i = 0; i < c->functions[function_id].total_vars; ++i) {
		if(!strcmp(name, c->functions[function_id].vars[i].name)) {
			location = c->functions[function_id].vars[i].ram_location;
			break;
		}
	}
	
	// if the variable is found and we need to only access one element we have to copy the array
	// value out into a temp place
	if(location >= 0 && element != NULL) {
		int var_dest = ++var_temp;
		int var_dimention = delta_compile_line_part(c, element, strlen(element));
		
		DELTA_WRITE_BYTECODE(BYTECODE_AG1, "", function_id,
							 new_DeltaInstruction3(NULL, BYTECODE_AG1, var_dest, location,
												   var_dimention));
		return var_dest;
	}
	
	// variable not found
	return location;
}


/**
 * @url http://en.wikipedia.org/wiki/Order_of_operations
 */
int delta_get_operator_order(char* op)
{
	if(!strcmp(op, "!"))
		return 2;
	if(!strcmp(op, "*") || !strcmp(op, "/") || !strcmp(op, "%"))
		return 3;
	if(!strcmp(op, "+") || !strcmp(op, "-"))
		return 4;
	if(!strcmp(op, ">>") || !strcmp(op, "<<"))
		return 5;
	if(!strcmp(op, "<") || !strcmp(op, "<=") || !strcmp(op, ">") || !strcmp(op, ">="))
		return 6;
	if(!strcmp(op, "==") || !strcmp(op, "!="))
		return 7;
	if(!strcmp(op, "&"))
		return 8;
	if(!strcmp(op, "^"))
		return 9;
	if(!strcmp(op, "|"))
		return 10;
	if(!strcmp(op, "&&"))
		return 11;
	if(!strcmp(op, "||"))
		return 12;
	if(!strcmp(op, "=") || !strcmp(op, "+=") || !strcmp(op, "*=") || !strcmp(op, "/=") ||
	   !strcmp(op, "%=") || !strcmp(op, "&=") || !strcmp(op, "|=") || !strcmp(op, "^=") ||
	   !strcmp(op, "<<=") || !strcmp(op, ">>="))
		return 14;
	
	// unknown operators are always performed with the lowest priority
	return 0;
}


int delta_is_user_function(struct DeltaCompiler *c, char *name)
{
	int i;
	for(i = 0; i < c->total_delta_functions; ++i) {
		if(!strcmp(c->delta_functions[i].name, name))
			return 0;
	}
	
	return 1;
}


char* delta_read_token(struct DeltaCompiler *c, int function_id, char* line, int* offset)
{
	int orig = *offset, len = strlen(line);
	
	// look for () with function
	int found = -1;
	int isNew = 0;
	for(; *offset < len; ++*offset) {
		if(isspace(line[*offset]) && !strncmp(line + *offset - 3, "new", 3)) {
			// found the new operator, need to create an Object
			isNew = 1;
			orig += 4;
			continue;
		}
		else if(line[*offset] == '[') {
			for(; *offset < len; ++*offset) {
				if(line[*offset] == ']')
					break;
			}
			++*offset;
		}
		else if(line[*offset] == '(') {
			found = *offset - orig;
			break;
		}
		if(!isalnum(line[*offset]) && line[*offset] != '_' &&
		   line[*offset] != '+' && line[*offset] != '-' && line[*offset] != '.' &&
		   line[*offset] != 'e' && line[*offset] != '$')
			break;
	}
	
	if(found >= 0) {
		char *function_name = NULL;
		if(isNew) {
			function_name = (char*) malloc(found + found + 2);
			strncpy(function_name, line + orig, found);
			strncpy(function_name + found, ".", 1);
			strncpy(function_name + found + 1, line + orig, found);
		}
		else {
			function_name = (char*) malloc(found + 1);
			strncpy(function_name, line + orig, found);
		}
		
		printf("'%s'\n", function_name);
		orig += found;
		
		// count the length of the subexpression
		int bcount_1 = 0; // ()
		for(; *offset < len; ++*offset) {
			if(line[*offset] == '(')
				++bcount_1;
			else if(line[*offset] == ')') {
				--bcount_1;
				if(!bcount_1)
					break;
			}
		}
		++*offset;
		++subexpression_depth;
		
		// evaluate the subexpression
		char* r = (char*) malloc(*offset - orig - 1);
		strncpy(r, line + orig + 1, *offset - orig - 2);
		delta_compile_line(c, r, strlen(r));
		
		// if there was a function, apply it now
		if(found > 0) {
			int var_dest = ++var_temp;
			arg_ptr[arg_depth][0] = var_dest;
			DELTA_WRITE_BYTECODE(BYTECODE_CAL, function_name, function_id,
								 new_DeltaInstructionN(function_name, BYTECODE_CAL));
			
			// move the return register back into the parent function only if it is a user function
			if(delta_is_user_function(c, function_name)) {
				DELTA_WRITE_BYTECODE(BYTECODE_SET, function_name, function_id,
									 new_DeltaInstruction2(NULL, BYTECODE_SET, var_dest,
														   RETURN_REGISTER));
			}
			
			r = (char*) malloc(8);
			sprintf(r, "#%d", var_dest);
		} else {
			r = (char*) malloc(8);
			// depth    offset
			// 3     -> -1
			// 4     -> -3
			// 5     -> -5
			// ((depth - 3) * 2)
			//int subexp = ((subexpression_depth - 3) * 2) + 1;
			//printf("subexpression_depth = %d -> %d\n", subexpression_depth, -subexp);
			//var_temp -= subexp;
			sprintf(r, "#%d", var_temp - 1);
		}
		--subexpression_depth;
		
		return r;
	}
	
	// look for a string constant
	if(line[*offset] == '"') {
		++*offset;
		
		// count the length of the string constant
		for(; *offset < len; ++*offset) {
			if(line[*offset] == '"')
				break;
		}
		++*offset;
		
		char* r = (char*) malloc(*offset - orig + 1);
		strncpy(r, line + orig, *offset - orig);
		return r;
	}
	
	// look for a string literal constant
	if(line[*offset] == '\'') {
		++*offset;
		
		// count the length of the string constant
		for(; *offset < len; ++*offset) {
			if(line[*offset] == '\'')
				break;
		}
		++*offset;
		
		char* r = (char*) malloc(*offset - orig + 1);
		strncpy(r, line + orig, *offset - orig);
		return r;
	}
	
	for(; *offset < len; ++*offset) {
		if(!isalnum(line[*offset]) && line[*offset] != '_')
			break;
	}
	
	char* r = (char*) malloc(orig + 1);
	strncpy(r, line + orig, *offset - orig);
	return r;
}

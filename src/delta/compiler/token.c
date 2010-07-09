/*
 *  Delta programming language
 */

#include "token.h"
#include "compiler.h"
#include "bytecode.h"
#include "delta/structs/DeltaInstruction.h"
#include "delta/structs/DeltaFunction.h"
#include "strings.h"
#include <string.h>
#include <ctype.h>


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
	return !strcmp(word, "return");
}


int delta_is_declared(struct DeltaCompiler *c, char* varname)
{
	int i;
	for(i = 0; i < c->total_vars; ++i) {
		if(!strcmp(c->vars[i].name, varname))
			return 1;
	}
	return 0;
}


int delta_get_variable_id(struct DeltaCompiler *c, char* name)
{
	// safety
	if(name == NULL)
		return -1;
	
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
	for(i = 0; i < c->total_vars; ++i) {
		if(!strcmp(name, c->vars[i].name)) {
			location = c->vars[i].ram_location;
			break;
		}
	}
	
	// if the variable is found and we need to only access one element we have to copy the array
	// value out into a temp place
	if(location >= 0 && element != NULL) {
		int var_dest = var_temp++;
		int var_dimention = delta_compile_line_part(c, element, strlen(element));
		
#if DELTA_SHOW_BYTECODE
		printf("BYTECODE_AG1 ( %d %d %d )\n", var_dest, location, var_dimention);
#endif
		DeltaFunction_push(c, new_DeltaInstruction3(NULL, BYTECODE_AG1, var_dest, location, var_dimention));
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


char* delta_read_token(struct DeltaCompiler *c, char* line, int* offset)
{
	int orig = *offset, len = strlen(line);
	
	// look for () with function
	int found = -1;
	for(; *offset < len; ++*offset) {
		if(line[*offset] == '[') {
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
		   line[*offset] != 'e')
			break;
	}
	
	if(found >= 0) {
		char *function_name = (char*) malloc(found + 1);
		strncpy(function_name, line + orig, found);
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
		
		// evaluate the subexpression
		char* r = (char*) malloc(*offset - orig - 1);
		strncpy(r, line + orig + 1, *offset - orig - 2);
		delta_compile_line(c, r, strlen(r));
		
		// if there was a function, apply it now
		if(found > 0) {
			int var_dest = ++var_temp;
			
			printf("BYTECODE_CAL %s(", function_name);
			int _k;
			arg_ptr[arg_depth][0] = var_dest;
			for(_k = 0; _k < arg_count[arg_depth]; ++_k) {
				printf(" %d", arg_ptr[arg_depth][_k]);
			}
			printf(" )\n");
			DeltaFunction_push(c, new_DeltaInstructionN(function_name, BYTECODE_CAL));
			
			r = (char*) malloc(8);
			sprintf(r, "#%d", var_dest);
		} else {
			r = (char*) malloc(8);
			sprintf(r, "#%d", var_temp);
		}
		
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
	
	for(; *offset < len; ++*offset) {
		if(!isalnum(line[*offset]) && line[*offset] != '_')
			break;
	}
	
	char* r = (char*) malloc(orig + 1);
	strncpy(r, line + orig, *offset - orig);
	return r;
}

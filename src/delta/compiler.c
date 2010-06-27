/*
 *  bytecode_compiler.c
 *  jit
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#include "compiler.h"
#include "bytecode.h"
#include <string.h>
#include <ctype.h>

#define DELTA_SHOW_BYTECODE 1

static int var_temp = 0;


DI new_DeltaInstruction0(DeltaByteCode bc)
{
	DI d;
	d.bc = bc;
	return d;
}


DI new_DeltaInstruction1(DeltaByteCode bc, int destination)
{
	DI d;
	d.bc = bc;
	d.destination = destination;
	return d;
}


DI new_DeltaInstruction2(DeltaByteCode bc, int destination, int source1)
{
	DI d;
	d.bc = bc;
	d.destination = destination;
	d.source1 = source1;
	return d;
}


DI new_DeltaInstruction3(DeltaByteCode bc, int destination, int source1, int source2)
{
	DI d;
	d.bc = bc;
	d.destination = destination;
	d.source1 = source1;
	d.source2 = source2;
	return d;
}


char* read_inv_token(char* line, int* offset)
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


void skip_spaces(char* line, int* offset)
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


int delta_is_declared(DeltaCompiler *c, char* varname)
{
	int i;
	for(i = 0; i < c->total_vars; ++i) {
		if(!strcmp(c->vars[i].name, varname))
			return 1;
	}
	return 0;
}


int delta_is_number(char *word)
{
	int i, len = strlen(word);
	for(i = 0; i < len; ++i) {
		if(!isnumber(word[i]) && word[i] != '.')
			return 0;
	}
	return 1;
}


int get_variable_id(DeltaCompiler *c, char* name)
{
	// safety
	if(name == NULL)
		return -1;
	
	// a register address
	if(name[0] == '#') {
		int temp = -1;
		sscanf(name, "#%d", &temp);
		return temp;
	}
	
	int i;
	for(i = 0; i < c->total_vars; ++i) {
		if(!strcmp(name, c->vars[i].name))
			return c->vars[i].ram_location;
	}
	
	// variable not found
	return -1;
}


int get_operator_order(char* op)
{
	if(!strcmp(op, "*") || !strcmp(op, "/"))
		return 1;
	if(!strcmp(op, "+") || !strcmp(op, "-"))
		return 2;
	if(!strcmp(op, "=") || !strcmp(op, "+="))
		return 3;
	
	// unknown operators are always performed with the lowest priority
	return 0;
}


int push_label(DeltaCompiler *c, char *name)
{
	return 0;
}


DeltaFunction* new_DeltaFunction()
{
	DeltaFunction *f = (DeltaFunction*) malloc(sizeof(DeltaFunction));	
	return f;
}


void DeltaFunction_push(DeltaCompiler* c, DI ins)
{
	c->ins[c->total_ins++] = ins;
}


DeltaObject* new_DeltaObject(int total_functions)
{
	DeltaObject *o = (DeltaObject*) malloc(sizeof(DeltaObject));
	o->total_functions = total_functions;
	o->f = (DeltaFunction**) malloc(o->total_functions * sizeof(DeltaFunction*));
	
	int i;
	for(i = 0; i < o->total_functions; ++i)
		o->f[i] = new_DeltaFunction();
	
	return o;
}


DeltaCompiler* new_DeltaCompiler(int total_objects)
{
	DeltaCompiler *c = (DeltaCompiler*) malloc(sizeof(DeltaCompiler));
	
	c->alloc_ins = 100;
	c->total_ins = 0;
	c->ins = (DI*) malloc(c->alloc_ins * sizeof(struct DeltaInstruction));
	
	c->alloc_vars = 100;
	c->total_vars = 0;
	c->vars = (DeltaVariable*) malloc(c->alloc_vars * sizeof(DeltaVariable));
	
	c->alloc_labels = 100;
	c->total_labels = 0;
	c->labels = (DeltaLabel*) malloc(c->alloc_labels * sizeof(DeltaLabel));
	
	c->alloc_constants = 100;
	c->total_constants = 0;
	c->constants = (DeltaVariable*) malloc(c->alloc_constants * sizeof(DeltaVariable));
	
	return c;
}


void delta_die(const char* msg)
{
	perror(msg);
	exit(1);
}


int delta_is_string(char* test)
{
	if(test == NULL)
		return DELTA_NO;
	if(test[0] == '"')
		return DELTA_YES;
	return DELTA_NO;
}


char* delta_copy_string(char* str)
{
	int len = strlen(str);
	char *r = (char*) malloc(len + 1);
	strcpy(r, str);
	return r;
}


char* delta_copy_substring(char* str, int start, int length)
{
	int len = length - start + 1;
	char *r = (char*) malloc(len + 1);
	strncpy(r, str + start, len);
	return r;
}


char* read_token(DeltaCompiler *c, char* line, int* offset)
{
	int orig = *offset, len = strlen(line);
	
	// look for () with function
	int found = -1;
	for(; *offset < len; ++*offset) {
		if(line[*offset] == '(') {
			found = *offset - orig;
			break;
		}
		if(!isalnum(line[*offset]))
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
		int result = delta_compile_line(c, r, strlen(r));
		printf("result = %d\n", result);
		
		// if there was a function, apply it now
		if(found > 0) {
			int var_dest = ++var_temp, var_id1 = result;
			
			if(!strcmp(function_name, "sqrt")) {
#if DELTA_SHOW_BYTECODE
				printf("BYTECODE_SQT (%d, %d)\n", var_dest, var_id1);
#endif
				DeltaFunction_push(c, new_DeltaInstruction2(BYTECODE_SQT, var_dest, var_id1));
			}
			else if(!strcmp(function_name, "cos")) {
#if DELTA_SHOW_BYTECODE
				printf("BYTECODE_COS (%d, %d)\n", var_dest, var_id1);
#endif
				DeltaFunction_push(c, new_DeltaInstruction2(BYTECODE_COS, var_dest, var_id1));
			}
			else if(!strcmp(function_name, "sin")) {
#if DELTA_SHOW_BYTECODE
				printf("BYTECODE_SIN (%d, %d)\n", var_dest, var_id1);
#endif
				DeltaFunction_push(c, new_DeltaInstruction2(BYTECODE_SIN, var_dest, var_id1));
			}
			else if(!strcmp(function_name, "tan")) {
#if DELTA_SHOW_BYTECODE
				printf("BYTECODE_TAN (%d, %d)\n", var_dest, var_id1);
#endif
				DeltaFunction_push(c, new_DeltaInstruction2(BYTECODE_TAN, var_dest, var_id1));
			}
			else if(!strcmp(function_name, "print")) {
#if DELTA_SHOW_BYTECODE
				printf("BYTECODE_OUT (%d, %d)\n", var_dest, var_id1);
#endif
				DeltaFunction_push(c, new_DeltaInstruction2(BYTECODE_OUT, var_dest, var_id1));
			}
			else {
				printf("Can't find function '%s'!\n", function_name);
				exit(1);
			}
			
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
		if(!isalnum(line[*offset]))
			break;
	}
	
	char* r = (char*) malloc(orig + 1);
	strncpy(r, line + orig, *offset - orig);
	return r;
}


int delta_compile_line(DeltaCompiler *c, char* line, int length)
{
	char *token, **tokens = (char**) malloc(64 * sizeof(char*));
	int i = 0, j, k, total_tokens = 0;
	
	if(line[0] == '#')
		return var_temp;
	
	// first parse the line and look for variables and constants
	while(i < length) {
		// skip any spaces before token
		skip_spaces(line, &i);
		
		token = read_token(c, line, &i);
		if(strlen(token) > 0)
			tokens[total_tokens++] = token;
		
		if(delta_is_string(token)) {
			++var_temp;
			c->constants[c->total_constants].type = DELTA_TYPE_STRING;
			c->constants[c->total_constants].value.ptr =
				delta_copy_substring(token, 1, strlen(token) - 2);
			c->constants[c->total_constants].ram_location = var_temp;
			sprintf(token, "#%d", var_temp);
			++c->total_constants;
		}
		else if(delta_is_number(token)) {
			++var_temp;
			c->constants[c->total_constants].type = DELTA_TYPE_NUMBER;
			c->constants[c->total_constants].value.number = atof(token);
			c->constants[c->total_constants].ram_location = var_temp;
			sprintf(token, "#%d", var_temp);
			++c->total_constants;
		}
		else if(!delta_is_keyword(token) && !delta_is_declared(c, token)) {
			c->vars[c->total_vars].type = DELTA_TYPE_NUMBER;
			c->vars[c->total_vars].name = (char*) malloc(strlen(token) + 1);
			strcpy(c->vars[c->total_vars].name, token);
			c->vars[c->total_vars].ram_location = var_temp++;
			++c->total_vars;
		}
		
		// skip any spaces before inverse token (aka operator)
		skip_spaces(line, &i);
		
		token = read_inv_token(line, &i);
		if(strlen(token) > 0)
			tokens[total_tokens++] = token;
	}
	
	if(total_tokens == 1) {
		// because we cant risk a constant being modified we will copy any constants out into a new
		// address
		int var_id1;
		if(delta_is_declared(c, tokens[0]))
			var_id1 = get_variable_id(c, tokens[0]);
		else
			var_id1 = var_temp - 1;
		
		return var_id1;
	}
	
	if(!strcmp(tokens[0], "if")) {
		int var_dest = var_temp++;
		int var_id1 = get_variable_id(c, tokens[1]);
		int var_id2 = get_variable_id(c, tokens[2]);
		
#if DELTA_SHOW_BYTECODE
		printf("BYTECODE_CLT (%d, %d, %d)\n", var_dest, var_id1, var_id2);
#endif
		DeltaFunction_push(c, new_DeltaInstruction3(BYTECODE_CLT, var_dest, var_id1, var_id2));
		
		var_id1 = var_dest;
		var_dest = 0; // label_id
#if DELTA_SHOW_BYTECODE
		printf("BYTECODE_IFS (%d, %d)\n", var_dest, var_id1);
#endif
		DeltaFunction_push(c, new_DeltaInstruction2(BYTECODE_IFS, var_dest, var_id1));
	}
	
	if(!strcmp(tokens[0], "label")) {
		int label_id = push_label(c, tokens[1]);
#if DELTA_SHOW_BYTECODE
		printf("BYTECODE_LBL (%d)\n", label_id);
#endif
		DeltaFunction_push(c, new_DeltaInstruction1(BYTECODE_LBL, label_id));
	}
	
	if(!strcmp(tokens[0], "return")) {
		if(total_tokens == 1) {
			// TODO: return 0
#if DELTA_SHOW_BYTECODE
			printf("BYTECODE_RTN (%d)\n", 0);
#endif
			DeltaFunction_push(c, new_DeltaInstruction1(BYTECODE_RTN, 0));
		} else {
			int var_id1 = get_variable_id(c, tokens[1]);
#if DELTA_SHOW_BYTECODE
			printf("BYTECODE_RTN (%d)\n", var_id1);
#endif
			DeltaFunction_push(c, new_DeltaInstruction1(BYTECODE_RTN, var_id1));
		}
	}
	
	// convert to bytecode
	i = 0;
	while(total_tokens > 2) {
		// find the most important operator
		int highest_op_pos = 1, highest_op_val = 100;
		for(j = 1; j < total_tokens; j += 2) {
			//printf("testing operator '%s' %d\n", tokens[j], get_operator_order(tokens[j]));
			if(get_operator_order(tokens[j]) < highest_op_val) {
				highest_op_val = get_operator_order(tokens[j]);
				highest_op_pos = j;
			}
		}
		
		// 2 argument operators
		if(!strcmp(tokens[highest_op_pos], "=")) {
			// resolve the address for the left and right
			int var_dest = get_variable_id(c, tokens[highest_op_pos - 1]);
			int var_id2 = get_variable_id(c, tokens[highest_op_pos + 1]);
			if(var_dest < 0) {
				fprintf(stderr, "Cannot resolve or write to '%s'\n", tokens[highest_op_pos - 1]);
				exit(1);
			}
			
#if DELTA_SHOW_BYTECODE
			printf("BYTECODE_SET (%d, %d)\n", var_dest, var_id2);
#endif
			DeltaFunction_push(c, new_DeltaInstruction2(BYTECODE_SET, var_dest, var_id2));
		}
		
		// 3 argument operators
		if(!strcmp(tokens[highest_op_pos], "+") ||
		   !strcmp(tokens[highest_op_pos], "-") ||
		   !strcmp(tokens[highest_op_pos], "*") ||
		   !strcmp(tokens[highest_op_pos], "/")) {
			// resolve the address for the left and right
			int var_dest = ++var_temp;
			int var_id1 = get_variable_id(c, tokens[highest_op_pos - 1]);
			int var_id2 = get_variable_id(c, tokens[highest_op_pos + 1]);
			
			if(!strcmp(tokens[highest_op_pos], "+")) {
#if DELTA_SHOW_BYTECODE
				printf("BYTECODE_ADD (%d, %d, %d)\n", var_dest, var_id1, var_id2);
#endif
				DeltaFunction_push(c, new_DeltaInstruction3(BYTECODE_ADD, var_dest, var_id1, var_id2));
			}
			else if(!strcmp(tokens[highest_op_pos], "-")) {
#if DELTA_SHOW_BYTECODE
				printf("BYTECODE_SUB (%d, %d, %d)\n", var_dest, var_id1, var_id2);
#endif
				DeltaFunction_push(c, new_DeltaInstruction3(BYTECODE_SUB, var_dest, var_id1, var_id2));
			}
			else if(!strcmp(tokens[highest_op_pos], "*")) {
#if DELTA_SHOW_BYTECODE
				printf("BYTECODE_MUL (%d, %d, %d)\n", var_dest, var_id1, var_id2);
#endif
				DeltaFunction_push(c, new_DeltaInstruction3(BYTECODE_MUL, var_dest, var_id1, var_id2));
			}
			else if(!strcmp(tokens[highest_op_pos], "/")) {
#if DELTA_SHOW_BYTECODE
				printf("BYTECODE_DIV (%d, %d, %d)\n", var_dest, var_id1, var_id2);
#endif
				DeltaFunction_push(c, new_DeltaInstruction3(BYTECODE_DIV, var_dest, var_id1, var_id2));
			}
		}
		
		if(!strcmp(tokens[highest_op_pos], "+=")) {
			// resolve the address for the left and right
			int var_id1 = get_variable_id(c, tokens[highest_op_pos - 1]);
			int var_id2 = get_variable_id(c, tokens[highest_op_pos + 1]);
			
			// var_id1 must resolve
			if(var_id1 < 0) {
				perror("LHS of += must be a variable\n");
				exit(1);
			}
			
#if DELTA_SHOW_BYTECODE
			printf("BYTECODE_ADD (%d, %d, %d)\n", var_id1, var_id1, var_id2);
#endif
			DeltaFunction_push(c, new_DeltaInstruction3(BYTECODE_ADD, var_id1, var_id1, var_id2));
		}
		
		// reduce temp
		tokens[highest_op_pos - 1] = (char*) malloc(6);
		sprintf(tokens[highest_op_pos - 1], "#%d", var_temp);
		
		/*for(k = 0; k < total_tokens; ++k) {
			printf(" '%s'", tokens[k]);
		}
		printf("\n");*/
		
		for(k = 0; k < total_tokens - highest_op_pos - 2; ++k) {
			tokens[highest_op_pos + k] = tokens[highest_op_pos + k + 2];
		}
		
		i += 2;
		total_tokens -= 2;
	}
	
	return var_temp;
}


int delta_compile_block(DeltaCompiler *c, char* block, int start, int end)
{
	// prepare
	char* line = (char*) malloc(1024);
	int i, line_pos = 0;
	
	// dissect lines
	for(i = start; i < end; ++i) {
		if(block[i] == ';') {
			line[line_pos] = 0;
			delta_compile_line(c, line, line_pos);
			line_pos = 0;
		} else
			line[line_pos++] = block[i];
	}
	
	// clean up
	free(line);
	return DELTA_SUCCESS;
}


int delta_compile_file(DeltaCompiler *c, const char* input_file)
{
	// prepare
	FILE *f = fopen(input_file, "r");
	if(f == NULL)
		delta_die("Could not open input file\n");
	
	// read the contents of the file
	fseek(f, 0, SEEK_END);
	long total_bytes = ftell(f);
	char *whole_file = (char*) malloc(total_bytes);
	fseek(f, 0, SEEK_SET);
	fread(whole_file, total_bytes, 1, f);
	
	// compile as a block
	delta_compile_block(c, whole_file, 0, total_bytes);
	
	// clean up
	fclose(f);
	free(whole_file);
	return DELTA_SUCCESS;
}

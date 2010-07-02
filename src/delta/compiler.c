/*
 *  Delta programming language
 */

#include "compiler.h"
#include "bytecode.h"
#include "vm.h"
#include <string.h>
#include <ctype.h>


#define DELTA_SHOW_BYTECODE 1
#define DELTA_MAX_NESTED_FUNCTIONS 16
#define DELTA_MAX_FUNCTION_ARGS 16


static int var_temp = 0;
static int *arg_count = NULL;
static int **arg_ptr = NULL;
static int arg_depth = 0;


#define DELTA_ADD_BYTECODE(__BYTECODE) \
	printf("BYTECODE_%s (", #__BYTECODE); \
	int _k; \
	arg_ptr[arg_depth][0] = var_dest; \
	for(_k = 0; _k < arg_count[arg_depth]; ++_k) { \
		printf(" %d", arg_ptr[arg_depth][_k]); \
	} \
	printf(" )\n"); \
	DeltaFunction_push(c, new_DeltaInstructionN(BYTECODE_##__BYTECODE));

#define DELTA_ADD_OPERATOR_BYTECODE(__BYTECODE) \
	printf("BYTECODE_%s (%d, %d, %d)\n", #__BYTECODE, var_dest, var_id1, var_id2); \
	DeltaFunction_push(c, new_DeltaInstruction3(NULL, BYTECODE_##__BYTECODE, var_dest, var_id1, var_id2));


DI new_DeltaInstruction0(char *name, DeltaByteCode bc)
{
	DI d;
	d.func = name;
	d.bc = bc;
	
	return d;
}


DI new_DeltaInstruction1(char *name, DeltaByteCode bc, int destination)
{
	DI d;
	d.func = name;
	d.bc = bc;
	d.args = 1;
	d.arg = (int*) malloc(d.args * sizeof(int));
	d.arg[0] = destination;
	
	return d;
}


DI new_DeltaInstruction2(char *name, DeltaByteCode bc, int destination, int source1)
{
	DI d;
	d.func = name;
	d.bc = bc;
	d.args = 2;
	d.arg = (int*) malloc(d.args * sizeof(int));
	d.arg[0] = destination;
	d.arg[1] = source1;
	
	return d;
}


DI new_DeltaInstruction3(char *name, DeltaByteCode bc, int destination, int source1, int source2)
{
	DI d;
	d.func = name;
	d.bc = bc;
	d.args = 3;
	d.arg = (int*) malloc(d.args * sizeof(int));
	d.arg[0] = destination;
	d.arg[1] = source1;
	d.arg[2] = source2;
	
	return d;
}


DI new_DeltaInstructionN(char *name, DeltaByteCode bc)
{
	DI d;
	d.func = name;
	d.bc = bc;
	
	// we don't just assign the pointer we copy the data so that theres no danger of the vaues
	// changing later and also it saves space.
	d.args = arg_count[arg_depth];
	d.arg = (int*) calloc(d.args, sizeof(int));
	int i;
	for(i = 0; i < d.args; ++i)
		d.arg[i] = arg_ptr[arg_depth][i];
	
	return d;
}


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
		if(!isnumber(word[i]) && word[i] != '.' && word[i] != '-')
			return 0;
	}
	return 1;
}


int delta_get_variable_id(DeltaCompiler *c, char* name)
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
	printf("looking for '%s' [%s]\n", name, element);
	
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


int delta_get_operator_order(char* op)
{
	if(!strcmp(op, "*") || !strcmp(op, "/"))
		return 1;
	if(!strcmp(op, "+") || !strcmp(op, "-"))
		return 2;
	if(!strcmp(op, "=") || !strcmp(op, "+=") || !strcmp(op, "-=") || !strcmp(op, "*=") ||
	   !strcmp(op, "/="))
		return 3;
	
	// unknown operators are always performed with the lowest priority
	return 0;
}


int delta_push_label(DeltaCompiler *c, char *name)
{
	return 0;
}


void DeltaFunction_push(DeltaCompiler* c, DI ins)
{
	c->ins[c->total_ins++] = ins;
}


DeltaCompiler* new_DeltaCompiler(int total_objects)
{
	DeltaCompiler *c = (DeltaCompiler*) malloc(sizeof(DeltaCompiler));
	
	c->alloc_ins = 100;
	c->total_ins = 0;
	c->ins = (DI*) malloc(c->alloc_ins * sizeof(struct DeltaInstruction));
	
	c->alloc_vars = 100;
	c->total_vars = 0;
	c->vars = (struct DeltaVariable*) malloc(c->alloc_vars * sizeof(struct DeltaVariable));
	
	c->alloc_labels = 100;
	c->total_labels = 0;
	c->labels = (DeltaLabel*) malloc(c->alloc_labels * sizeof(DeltaLabel));
	
	c->alloc_constants = 100;
	c->total_constants = 0;
	c->constants = (struct DeltaVariable*) malloc(c->alloc_constants * sizeof(struct DeltaVariable));
	
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
	int len = length - start;
	char *r = (char*) malloc(len + 1);
	strncpy(r, str + start, len);
	return r;
}


char* delta_read_token(DeltaCompiler *c, char* line, int* offset)
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
		   line[*offset] != '+' && line[*offset] != '-')
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


int delta_push_constant(DeltaCompiler *c, char *token)
{
	++var_temp;
	c->constants[c->total_constants].type = DELTA_TYPE_STRING;
	c->constants[c->total_constants].value.ptr = token;
	c->constants[c->total_constants].ram_location = var_temp;
	c->constants[c->total_constants].size = strlen(token);
	++c->total_constants;
	return var_temp;
}


int delta_compile_line_part(DeltaCompiler *c, char* line, int length)
{
	char *token, **tokens = (char**) malloc(64 * sizeof(char*));
	int i, j, k, total_tokens = 0;
	
	// first parse the line and look for variables and constants
	for(i = 0; i < length; ) {
		// skip any spaces before token
		delta_skip_spaces(line, &i);
		
		token = delta_read_token(c, line, &i);
		if(strlen(token) > 0)
			tokens[total_tokens++] = token;
		
		if(delta_is_string(token)) {
			sprintf(token, "#%d", delta_push_constant(c, delta_copy_substring(token, 1, strlen(token) - 1)));
		} else if(delta_is_number(token)) {
			++var_temp;
			c->constants[c->total_constants].type = DELTA_TYPE_NUMBER;
			c->constants[c->total_constants].value.number = atof(token);
			c->constants[c->total_constants].ram_location = var_temp;
			delta_vm_print_variable(&c->constants[c->total_constants]);
			printf("\n");
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
		delta_skip_spaces(line, &i);
		
		token = delta_read_inv_token(line, &i);
		if(strlen(token) > 0)
			tokens[total_tokens++] = token;
	}
	
	if(total_tokens == 0)
		return var_temp;
	else if(total_tokens == 1) {
		// because we cant risk a constant being modified we will copy any constants out into a new
		// address
		int var_id1;
		if(delta_is_declared(c, tokens[0]))
			var_id1 = delta_get_variable_id(c, tokens[0]);
		else
			var_id1 = var_temp;
		
		return var_id1;
	}
	
	if(!strcmp(tokens[0], "return")) {
		if(total_tokens == 1) {
			// TODO: return 0
#if DELTA_SHOW_BYTECODE
			printf("BYTECODE_RTN (%d)\n", 0);
#endif
			DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_RTN, 0));
		} else {
			int var_id1 = delta_get_variable_id(c, tokens[1]);
#if DELTA_SHOW_BYTECODE
			printf("BYTECODE_RTN (%d)\n", var_id1);
#endif
			DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_RTN, var_id1));
		}
	}
	
	// convert to bytecode
	i = 0;
	while(total_tokens > 2) {
		// find the most important operator
		int highest_op_pos = 1, highest_op_val = 100;
		for(j = 1; j < total_tokens; j += 2) {
			if(delta_get_operator_order(tokens[j]) < highest_op_val) {
				highest_op_val = delta_get_operator_order(tokens[j]);
				highest_op_pos = j;
			}
		}
		
		// 2 argument operators
		if(!strcmp(tokens[highest_op_pos], "=")) {
			// resolve the address for the left and right
			//printf("'%s' = '%s'\n", tokens[highest_op_pos - 1], tokens[highest_op_pos + 1]);
			int var_dest;
			int var_id1 = delta_get_variable_id(c, tokens[highest_op_pos + 1]);
			
			if(delta_strpos(tokens[highest_op_pos - 1], "[") > 0) {
				var_dest = delta_get_variable_id(c, "a");
				if(var_dest < 0) {
					fprintf(stderr, "Cannot resolve or write to '%s'\n", tokens[highest_op_pos - 1]);
					exit(1);
				}
				
				// TODO: fixme
				char *line2 = "15";
				int var_dimention = delta_compile_line_part(c, line2, 2);
				
#if DELTA_SHOW_BYTECODE
				printf("BYTECODE_AS1 ( %d %d %d )\n", var_dest, var_dimention, var_id1);
#endif
				DeltaFunction_push(c, new_DeltaInstruction3(NULL, BYTECODE_AS1, var_dest, var_dimention, var_id1));
			} else {
				var_dest = delta_get_variable_id(c, tokens[highest_op_pos - 1]);
				if(var_dest < 0) {
					fprintf(stderr, "Cannot resolve or write to '%s'\n", tokens[highest_op_pos - 1]);
					exit(1);
				}
				
#if DELTA_SHOW_BYTECODE
				printf("BYTECODE_SET (%d, %d)\n", var_dest, var_id1);
#endif
				DeltaFunction_push(c, new_DeltaInstruction2(NULL, BYTECODE_SET, var_dest, var_id1));
			}
		}
		
		// 3 argument operators
		if(!strcmp(tokens[highest_op_pos], "+") ||
		   !strcmp(tokens[highest_op_pos], "-") ||
		   !strcmp(tokens[highest_op_pos], "*") ||
		   !strcmp(tokens[highest_op_pos], "/") ||
		   !strcmp(tokens[highest_op_pos], "<") ||
		   !strcmp(tokens[highest_op_pos], ">") ||
		   !strcmp(tokens[highest_op_pos], "<=") ||
		   !strcmp(tokens[highest_op_pos], ">=")) {
			// resolve the address for the left and right
			int var_dest = ++var_temp;
			int var_id1 = delta_get_variable_id(c, tokens[highest_op_pos - 1]);
			int var_id2 = delta_get_variable_id(c, tokens[highest_op_pos + 1]);
			
			if(!strcmp(tokens[highest_op_pos], "+")) {
				DELTA_ADD_OPERATOR_BYTECODE(ADD);
			}
			else if(!strcmp(tokens[highest_op_pos], "-")) {
				DELTA_ADD_OPERATOR_BYTECODE(SUB);
			}
			else if(!strcmp(tokens[highest_op_pos], "*")) {
				DELTA_ADD_OPERATOR_BYTECODE(MUL);
			}
			else if(!strcmp(tokens[highest_op_pos], "/")) {
				DELTA_ADD_OPERATOR_BYTECODE(DIV);
			}
			else if(!strcmp(tokens[highest_op_pos], "<")) {
				DELTA_ADD_OPERATOR_BYTECODE(CLT);
			}
			else if(!strcmp(tokens[highest_op_pos], ">")) {
				DELTA_ADD_OPERATOR_BYTECODE(CGT);
			}
			else if(!strcmp(tokens[highest_op_pos], "<=")) {
				DELTA_ADD_OPERATOR_BYTECODE(CLE);
			}
			else if(!strcmp(tokens[highest_op_pos], ">=")) {
				DELTA_ADD_OPERATOR_BYTECODE(CGE);
			}
		}
		
		if(!strcmp(tokens[highest_op_pos], "+=") ||
		   !strcmp(tokens[highest_op_pos], "-=") ||
		   !strcmp(tokens[highest_op_pos], "*=") ||
		   !strcmp(tokens[highest_op_pos], "/=")) {
			// resolve the address for the left and right
			int var_id1 = delta_get_variable_id(c, tokens[highest_op_pos - 1]);
			int var_id2 = delta_get_variable_id(c, tokens[highest_op_pos + 1]);
			int var_dest = var_id1;
			
			// var_id1 must resolve
			if(var_id1 < 0) {
				perror("LHS of += must be a variable\n");
				exit(1);
			}
			
			if(!strcmp(tokens[highest_op_pos], "+=")) {
				DELTA_ADD_OPERATOR_BYTECODE(ADD);
			}
			else if(!strcmp(tokens[highest_op_pos], "-=")) {
				DELTA_ADD_OPERATOR_BYTECODE(SUB);
			}
			else if(!strcmp(tokens[highest_op_pos], "*=")) {
				DELTA_ADD_OPERATOR_BYTECODE(MUL);
			}
			else if(!strcmp(tokens[highest_op_pos], "/=")) {
				DELTA_ADD_OPERATOR_BYTECODE(DIV);
			}
		}
		
		// reduce temp
		tokens[highest_op_pos - 1] = (char*) malloc(6);
		sprintf(tokens[highest_op_pos - 1], "#%d", var_temp);
		
		for(k = 0; k < total_tokens - highest_op_pos - 2; ++k) {
			tokens[highest_op_pos + k] = tokens[highest_op_pos + k + 2];
		}
		
		i += 2;
		total_tokens -= 2;
	}
	
	return var_temp;
}


char* delta_extract_argument_key(char *arg)
{
	// look for the operator
	int i, len = strlen(arg), found = -1;
	for(i = 0; i < len - 1; ++i) {
		if(arg[i] == '=' && arg[i + 1] == '>') {
			found = i;
			break;
		}
	}
	
	// a key was found
	if(found >= 0) {
		// chop the key off
		char *r = delta_copy_substring(arg, 0, found);
		
		// TODO: memory leak
		arg = delta_copy_substring(arg, found + 2, len);
		
		return r;
	}
	
	// no key was found
	return NULL;
}


int delta_compile_line(DeltaCompiler *c, char* line, int length)
{
	// split the line on commas
	int total_parts = 0, i, last = -1;
	int bcount1 = 0; // ()
	char **parts = (char**) malloc(64 * sizeof(char*));
	
	for(i = 0; i < length; ++i) {
		if(line[i] == '(')
			++bcount1;
		if(line[i] == ')')
			--bcount1;
		
		if((line[i] == ',' && !bcount1) || i == length - 1) {
			if(i == length - 1)
				++i;
				
			parts[total_parts] = (char*) malloc(i - last);
			strncpy(parts[total_parts], line + last + 1, i - last - 1);
			++total_parts;
			last = i;
		}
	}
	
	// for each part there will be a destination address
	arg_count[arg_depth] = (total_parts * 2) + 1;
	for(i = 0; i < total_parts; ++i) {
		++arg_depth;
		
		// create the argument key
		char *key = delta_extract_argument_key(parts[i]);
		int key_addr;
		if(key == NULL) {
			key = (char*) malloc(8);
			sprintf(key, "%d", i);
			key_addr = delta_push_constant(c, key);
		}
		else
			key_addr = delta_compile_line_part(c, key, strlen(key));

		
		arg_ptr[arg_depth - 1][(i * 2) + 1] = key_addr;
		arg_ptr[arg_depth - 1][(i * 2) + 2] = delta_compile_line_part(c, parts[i], strlen(parts[i]));
		--arg_depth;
	}
	arg_ptr[arg_depth][0] = var_temp++;
	
	return arg_ptr[arg_depth][0];
}


int delta_strpos(char *haystack, char *needle)
{
	char *p = strstr(haystack, needle);
	if (p)
		return p - haystack;
	return -1;
}


int delta_compile_block(DeltaCompiler *c, char *identifier, char *block, int start, int end)
{	
	// prepare
	char* line = (char*) malloc(1024);
	int i, line_pos = 0, ii = 0;
	
	// compile identifier
	delta_skip_spaces(identifier, &ii);
	int identifier_len;
	for(identifier_len = 0; identifier_len < strlen(identifier); ++identifier_len) {
		if(!isalnum(identifier[identifier_len + ii]))
			break;
	}
	
	char *short_identifier = (char*) malloc(identifier_len + 1);
	strncpy(short_identifier, identifier + ii, identifier_len);
	
	if(identifier_len == 0) {
		// do nothing
	}
	else if(!strcmp(short_identifier, "else")) {
		// do nothing
	}
	else if(!strcmp(short_identifier, "if")) {
		// if statement, get the conditional expression
		int expr_start = delta_strpos(identifier, "(") + 1;
		int expr_end = delta_strpos(identifier, ")");
		char *expr = (char*) malloc(expr_end - expr_start + 1);
		strncpy(expr, identifier + expr_start, expr_end - expr_start);
		
		// evaluate expression
		printf("expr = '%s'\n", expr);
		int expr_eval = delta_compile_line(c, expr, expr_end - expr_start);
		
		// perform if statement
		int label_id = 0;
#if DELTA_SHOW_BYTECODE
		printf("BYTECODE_IFS (%d, %d)\n", label_id, expr_eval);
#endif
		DeltaFunction_push(c, new_DeltaInstruction2(NULL, BYTECODE_IFS, label_id, expr_eval));
	}
	else {
		printf("Unknown block identifier '%s'", short_identifier);
		exit(0);
	}
	
	// dissect lines
	for(i = start; i < end; ++i) {
		if(block[i] == '{') {
			char *new_identifier = (char*) malloc(line_pos + 1);
			strncpy(new_identifier, block + i - line_pos, line_pos);
			
			// read subblock
			int orig = i + 1;
			for(; i < end; ++i) {
				if(block[i] == '}')
					break;
			}
			delta_compile_block(c, new_identifier, block, orig, i - 1);
			line_pos = 0;
		}
		else if(block[i] == ';') {
			line[line_pos] = 0;
			delta_compile_line(c, line, line_pos);
			line_pos = 0;
		}
		else
			line[line_pos++] = block[i];
	}
	
	// add forward patch
	if(!strcmp(short_identifier, "if")) {
		// add the jump for else
		printf("BYTECODE_JMP ()\n");
		DeltaFunction_push(c, new_DeltaInstruction0(NULL, BYTECODE_JMP));
		
		printf("BYTECODE_PAT ()\n");
		DeltaFunction_push(c, new_DeltaInstruction0(NULL, BYTECODE_PAT));
	}
	else if(!strcmp(short_identifier, "else")) {
		printf("BYTECODE_PAT ()\n");
		DeltaFunction_push(c, new_DeltaInstruction0(NULL, BYTECODE_PAT));
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
	arg_count = (int*) calloc(DELTA_MAX_NESTED_FUNCTIONS, sizeof(int));
	arg_ptr = (int**) calloc(DELTA_MAX_NESTED_FUNCTIONS, sizeof(int*));
	int i;
	for(i = 0; i < DELTA_MAX_NESTED_FUNCTIONS; ++i)
		arg_ptr[i] = (int*) calloc(DELTA_MAX_FUNCTION_ARGS, sizeof(int));
	delta_compile_block(c, "", whole_file, 0, total_bytes);
	
	// clean up
	fclose(f);
	free(whole_file);
	return DELTA_SUCCESS;
}

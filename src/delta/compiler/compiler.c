/*
 *  Delta programming language
 */

#include "compiler.h"
#include "bytecode.h"
#include "vm.h"
#include "strings.h"
#include "../macros.h"
#include <string.h>
#include <ctype.h>


#define DELTA_SHOW_BYTECODE 1
#define DELTA_MAX_NESTED_FUNCTIONS 16
#define DELTA_MAX_FUNCTION_ARGS 16
#define DELTA_INF 1e2000


static int var_temp = 0;
static int *arg_count = NULL;
static int **arg_ptr = NULL;
static int arg_depth = 0;
static int label_id = 0;


struct DeltaInstruction new_DeltaInstruction0(char *name, DeltaByteCode bc)
{
	struct DeltaInstruction d;
	d.func = name;
	d.bc = bc;
	
	return d;
}


struct DeltaInstruction new_DeltaInstruction1(char *name, DeltaByteCode bc, int destination)
{
	struct DeltaInstruction d;
	d.func = name;
	d.bc = bc;
	d.args = 1;
	d.arg = (int*) malloc(d.args * sizeof(int));
	d.arg[0] = destination;
	
	return d;
}


struct DeltaInstruction new_DeltaInstruction2(char *name, DeltaByteCode bc, int destination, int source1)
{
	struct DeltaInstruction d;
	d.func = name;
	d.bc = bc;
	d.args = 2;
	d.arg = (int*) malloc(d.args * sizeof(int));
	d.arg[0] = destination;
	d.arg[1] = source1;
	
	return d;
}


struct DeltaInstruction new_DeltaInstruction3(char *name, DeltaByteCode bc, int destination, int source1, int source2)
{
	struct DeltaInstruction d;
	d.func = name;
	d.bc = bc;
	d.args = 3;
	d.arg = (int*) malloc(d.args * sizeof(int));
	d.arg[0] = destination;
	d.arg[1] = source1;
	d.arg[2] = source2;
	
	return d;
}


struct DeltaInstruction new_DeltaInstructionN(char *name, DeltaByteCode bc)
{
	struct DeltaInstruction d;
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


int delta_push_label(struct DeltaCompiler *c, char *name)
{
	return 0;
}


void DeltaFunction_push(struct DeltaCompiler* c, struct DeltaInstruction ins)
{
	c->ins[c->total_ins++] = ins;
}


struct DeltaCompiler* new_DeltaCompiler(int total_objects)
{
	struct DeltaCompiler *c = (struct DeltaCompiler*) malloc(sizeof(struct DeltaCompiler));
	
	c->alloc_ins = 100;
	c->total_ins = 0;
	c->ins = (struct DeltaInstruction*) malloc(c->alloc_ins * sizeof(struct DeltaInstruction));
	
	c->alloc_vars = 100;
	c->total_vars = 0;
	c->vars = (struct DeltaVariable*) malloc(c->alloc_vars * sizeof(struct DeltaVariable));
	
	c->alloc_labels = 100;
	c->total_labels = 0;
	c->labels = (struct DeltaLabel*) malloc(c->alloc_labels * sizeof(struct DeltaLabel));
	
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


int delta_push_constant(struct DeltaCompiler *c, char *token)
{
	++var_temp;
	c->constants[c->total_constants].type = DELTA_TYPE_STRING;
	c->constants[c->total_constants].value.ptr = token;
	c->constants[c->total_constants].ram_location = var_temp;
	c->constants[c->total_constants].size = strlen(token);
	++c->total_constants;
	return var_temp;
}


int delta_push_number_constant(struct DeltaCompiler *c, double value)
{
	++var_temp;
	c->constants[c->total_constants].type = DELTA_TYPE_NUMBER;
	c->constants[c->total_constants].value.number = value;
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


int delta_compile_line_part(struct DeltaCompiler *c, char* line, int length)
{
	char *token, **tokens = (char**) malloc(64 * sizeof(char*));
	int i, j, k, total_tokens = 0;
	
	// check for negation
	if(line[0] == '-') {
		// FIXME: leak, should this even need to be here?
		char *new_line = (char*) malloc(length + 2);
		new_line[0] = '0';
		strncpy(new_line + 1, line, length);
		line = new_line;
	}
	
	// first parse the line and look for variables and constants
	for(i = 0; i < length; ) {
		// skip any spaces before token
		delta_skip_spaces(line, &i);
		
		token = delta_read_token(c, line, &i);
		token = delta_replace_constant(token);
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


int delta_compile_line(struct DeltaCompiler *c, char* line, int length)
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


int delta_compile_block(struct DeltaCompiler *c, char *identifier, char *block, int at, int end)
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
		int expr_at = delta_strpos(identifier, "(") + 1;
		int expr_end = delta_strrchr(identifier, ')');
		char *expr = (char*) malloc(expr_end - expr_at + 1);
		strncpy(expr, identifier + expr_at, expr_end - expr_at);
		
		// evaluate expression
		int expr_eval = delta_compile_line_part(c, expr, expr_end - expr_at);
		
		// perform if statement
		++label_id;
#if DELTA_SHOW_BYTECODE
		printf("BYTECODE_IFS (%d, %d)\n", label_id, expr_eval);
#endif
		DeltaFunction_push(c, new_DeltaInstruction2(NULL, BYTECODE_IFS, label_id, expr_eval));
	}
	else if(!strcmp(short_identifier, "while")) {
		// if statement, get the conditional expression
		int expr_at = delta_strpos(identifier, "(") + 1;
		int expr_end = delta_strrchr(identifier, ')');
		char *expr = (char*) malloc(expr_end - expr_at + 1);
		strncpy(expr, identifier + expr_at, expr_end - expr_at);
		
		// loop label
		printf("BYTECODE_LBL ( %d )\n", label_id);
		DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_LBL, label_id));
		
		// evaluate expression
		int expr_eval = delta_compile_line_part(c, expr, expr_end - expr_at);
		
		// perform if statement
		++label_id;
#if DELTA_SHOW_BYTECODE
		printf("BYTECODE_LOP (%d, %d)\n", label_id, expr_eval);
#endif
		DeltaFunction_push(c, new_DeltaInstruction2(NULL, BYTECODE_LOP, label_id, expr_eval));
	}
	else {
		printf("Unknown block identifier '%s'", short_identifier);
		exit(0);
	}
	
	// dissect lines
	for(i = at; i < end; ++i) {
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
		printf("BYTECODE_JMP ( %d )\n", label_id + 1);
		DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_JMP, label_id + 1));
		
		printf("BYTECODE_PAT ( %d )\n", label_id);
		DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_PAT, label_id));
		++label_id;
	}
	else if(!strcmp(short_identifier, "else")) {
		// patch from if jump
		printf("BYTECODE_PAT ( %d )\n", label_id);
		DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_PAT, label_id));
	}
	else if(!strcmp(short_identifier, "while")) {
		// jump back to the while expression for the next iteration
		printf("BYTECODE_GTO ( %d )\n", label_id);
		DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_GTO, label_id));
		
		// patch the forward jump when the loops ends
		printf("BYTECODE_PAT ( %d )\n", label_id);
		DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_PAT, label_id));
	}
	
	// clean up
	free(line);
	return DELTA_SUCCESS;
}


int delta_compile_file(struct DeltaCompiler *c, const char* input_file)
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


struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args)
{
	struct DeltaFunction *f = (struct DeltaFunction*) malloc(sizeof(struct DeltaFunction));
	
	f->name = name;
	f->function_ptr = function_ptr;
	f->min_args = min_args;
	f->max_args = max_args;
	
	return f;
}


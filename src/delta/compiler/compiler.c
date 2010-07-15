/*
 *  Delta programming language
 */

#include "compiler.h"
#include "bytecode.h"
#include "delta/vm/vm.h"
#include "delta/compiler/strings.h"
#include "delta/structs/DeltaInstruction.h"
#include "delta/structs/DeltaFunction.h"
#include "token.h"
#include "constant.h"
#include "delta/macros.h"
#include <string.h>
#include <ctype.h>


#define DELTA_SHOW_BYTECODE 1
#define DELTA_MAX_NESTED_FUNCTIONS 16
#define DELTA_MAX_FUNCTION_ARGS 16
#define DELTA_INF 1e2000


int var_temp = 0;
int *arg_count = NULL;
int **arg_ptr = NULL;
int arg_depth = 0;
int label_id = 0;


int delta_push_label(struct DeltaCompiler *c, char *name)
{
	return 0;
}


void delta_die(const char* msg)
{
	perror(msg);
	exit(1);
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
		
		if(delta_is_number(token)) {
			++var_temp;
			c->constants[c->total_constants].type = DELTA_TYPE_NUMBER;
			c->constants[c->total_constants].value.number = atof(token);
			c->constants[c->total_constants].ram_location = var_temp;
			delta_vm_print_variable(&c->constants[c->total_constants]);
			printf("\n");
			sprintf(token, "#%d", var_temp);
			++c->total_constants;
		}
		else if(delta_is_string(token)) {
			sprintf(token, "#%d", delta_push_constant(c, delta_copy_substring(token, 1, strlen(token) - 1)));
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
		   !strcmp(tokens[highest_op_pos], "%") ||
		   !strcmp(tokens[highest_op_pos], "<") ||
		   !strcmp(tokens[highest_op_pos], ">") ||
		   !strcmp(tokens[highest_op_pos], "<=") ||
		   !strcmp(tokens[highest_op_pos], ">=") ||
		   !strcmp(tokens[highest_op_pos], "==") ||
		   !strcmp(tokens[highest_op_pos], "!=")) {
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
			else if(!strcmp(tokens[highest_op_pos], "%")) {
				DELTA_ADD_OPERATOR_BYTECODE(MOD);
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
			else if(!strcmp(tokens[highest_op_pos], "==")) {
				DELTA_ADD_OPERATOR_BYTECODE(CEQ);
			}
			else if(!strcmp(tokens[highest_op_pos], "!=")) {
				DELTA_ADD_OPERATOR_BYTECODE(CNE);
			}
		}
		
		if(!strcmp(tokens[highest_op_pos], "+=") ||
		   !strcmp(tokens[highest_op_pos], "-=") ||
		   !strcmp(tokens[highest_op_pos], "*=") ||
		   !strcmp(tokens[highest_op_pos], "/=") ||
		   !strcmp(tokens[highest_op_pos], "%=")) {
			// resolve the address for the left and right
			int var_id1 = delta_get_variable_id(c, tokens[highest_op_pos - 1]);
			int var_id2 = delta_get_variable_id(c, tokens[highest_op_pos + 1]);
			int var_dest = var_id1;
			
			// var_id1 must resolve
			if(var_id1 < 0) {
				fprintf(stderr, "LHS of %s must be a variable\n", tokens[highest_op_pos]);
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
			else if(!strcmp(tokens[highest_op_pos], "%=")) {
				DELTA_ADD_OPERATOR_BYTECODE(MOD);
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
			int b1 = 0; // ()
			int b2 = 0; // []
			int b3 = 0; // {}
			for(; i < end; ++i) {
				if(block[i] == '(')
					++b1;
				else if(block[i] == ')')
					--b1;
				else if(block[i] == '[')
					++b2;
				else if(block[i] == ']')
					--b2;
				else if(block[i] == '{')
					++b3;
				else if(block[i] == '}')
					--b3;
				
				if(block[i] == '}' && !b1 && !b2 && !b3)
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
		//printf("BYTECODE_JMP ( %d )\n", label_id + 1);
		//DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_JMP, label_id + 1));
		
		printf("BYTECODE_PAT ( %d ) <- if\n", label_id);
		DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_PAT, label_id));
		--label_id;
	}
	else if(!strcmp(short_identifier, "else")) {
		// patch from if jump
		printf("BYTECODE_PAT ( %d ) <- else\n", label_id);
		DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_PAT, label_id));
		--label_id;
	}
	else if(!strcmp(short_identifier, "while")) {
		// jump back to the while expression for the next iteration
		printf("BYTECODE_GTO ( %d )\n", label_id - 1);
		DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_GTO, label_id - 1));
		
		// patch the forward jump when the loops ends
		printf("BYTECODE_PAT ( %d ) <- while\n", label_id);
		DeltaFunction_push(c, new_DeltaInstruction1(NULL, BYTECODE_PAT, label_id));
		--label_id;
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

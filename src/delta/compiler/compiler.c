/*
 *  Delta programming language
 */

#include "compiler.h"
#include "bytecode.h"
#include "delta/vm/vm.h"
#include "strings.h"
#include "errors.h"
#include "delta/structs/DeltaInstruction.h"
#include "delta/structs/DeltaFunction.h"
#include "delta/structs/DeltaCompiler.h"
#include "token.h"
#include "constant.h"
#include "delta/macros.h"
#include "delta/jit/optimiser.h"
#include "delta/compiler/bytecode_writer.h"
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <assert.h>


#define DELTA_MAX_NESTED_FUNCTIONS 16
#define DELTA_MAX_FUNCTION_ARGS 16
#define DELTA_INF 1e2000


int var_temp = 0, result_register = 0;
int *arg_count = NULL;
int **arg_ptr = NULL;
int arg_depth = 0;
int label_id = 0;
int subexpression_depth = 0;
int line_number = 1;
static char *class_name = NULL;


void delta_function_reset()
{
	var_temp = 0;
	result_register = 0;
	arg_depth = 0;
	label_id = 0;
	subexpression_depth = 1;
	arg_count = (int*) calloc(DELTA_MAX_NESTED_FUNCTIONS, sizeof(int));
	arg_ptr = (int**) calloc(DELTA_MAX_NESTED_FUNCTIONS, sizeof(int*));
	
	int i;
	for(i = 0; i < DELTA_MAX_NESTED_FUNCTIONS; ++i)
		arg_ptr[i] = (int*) calloc(DELTA_MAX_FUNCTION_ARGS, sizeof(int));
}


int delta_push_label(struct DeltaCompiler *c, char *name)
{
	// TODO: fixme
	return 0;
}


void delta_die(const char* msg)
{
	printf("%s", msg);
	exit(1);
}


int delta_compile_line_part(struct DeltaCompiler *c, char* line, int length)
{
	if(!strncmp(delta_trim(line), "return", 6)) {
		int pos = delta_strpos(line, "r") + 6;
		int var_id1 = delta_compile_line_part(c, line + pos, length - pos);
		
		DELTA_WRITE_BYTECODE(BYTECODE_SET, "", c->total_functions,
							 new_DeltaInstruction2(NULL, BYTECODE_SET, RETURN_REGISTER, var_id1));
		DELTA_WRITE_BYTECODE(BYTECODE_RTN, "", c->total_functions,
							 new_DeltaInstruction0(NULL, BYTECODE_RTN));
		
		return var_id1;
	}
	
	char *token, **tokens = (char**) malloc(64 * sizeof(char*));
	int i, j, k, total_tokens = 0;
	
	// check for negation
	if(line[0] == '-') {
		// FIXME: Issue #22: leak, should this even need to be here?
		char *new_line = (char*) malloc(length + 2);
		new_line[0] = '0';
		strncpy(new_line + 1, line, length);
		line = new_line;
	}
	
	// first parse the line and look for variables and constants
	for(i = 0; i < length; ) {
		// skip any spaces before token
		delta_skip_spaces(line, &i);
		
		token = delta_read_token(c, c->total_functions, line, &i);
		token = delta_replace_constant(c, token);
		if(strlen(token) > 0)
			tokens[total_tokens++] = token;
		
		if(delta_is_number(token)) {
			++var_temp;
			int total_constants = c->functions[c->total_functions].total_constants;
			c->functions[c->total_functions].constants[total_constants].type = DELTA_TYPE_NUMBER;
			c->functions[c->total_functions].constants[total_constants].value.number = atof(token);
			c->functions[c->total_functions].constants[total_constants].ram_location = var_temp;
			sprintf(token, "#%d", var_temp);
			++c->functions[c->total_functions].total_constants;
		}
		else if(delta_is_string(token)) {
			if(delta_is_magic_string(token)) {
				token = delta_translate_magic_string(token);
				printf("token = '%s'\n", token);
				int res_reg = delta_compile_line_part(c, token, strlen(token));
				sprintf(token, "#%d", res_reg);
			} else {
				int escape = (token[0] == '"');
				char *str = delta_copy_substring(token, 1, strlen(token) - 1);
				sprintf(token, "#%d", delta_push_constant(c, c->total_functions, str, escape));
			}
		}
		else if(!delta_is_keyword(token) && !delta_is_declared(c, c->total_functions, token)) {
			// make sure the variable name is valid
			if(tolower(token[0]) != token[0])
				delta_error_push(c, line_number,
								 "Variables names must start with a non-capital letter.");
				
			int total_vars = c->functions[c->total_functions].total_vars;
			c->functions[c->total_functions].vars[total_vars].type = DELTA_TYPE_NUMBER;
			c->functions[c->total_functions].vars[total_vars].name = (char*) malloc(strlen(token) + 1);
			strcpy(c->functions[c->total_functions].vars[total_vars].name, token);
			c->functions[c->total_functions].vars[total_vars].ram_location = ++var_temp;
			++c->functions[c->total_functions].total_vars; 
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
		int var_id1 = var_temp;
		
		if(tokens[0][0] == '#') {
			sscanf(tokens[0], "#%d", &var_temp);
			return var_temp;
		}
		
		if(delta_is_declared(c, c->total_functions, tokens[0]))
			var_id1 = delta_get_variable_id(c, c->total_functions, tokens[0]);
		
		return var_id1;
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
			int var_dest;
			int var_id1 = delta_get_variable_id(c, c->total_functions, tokens[highest_op_pos + 1]);
			
			if(delta_strpos(tokens[highest_op_pos - 1], "[") > 0) {
				var_dest = delta_get_variable_id(c, c->total_functions, "a");
				if(var_dest < 0) {
					fprintf(stderr, "Cannot resolve or write to '%s'\n",
							tokens[highest_op_pos - 1]);
					exit(1);
				}
				
				// TODO: fixme
				char *line2 = "15";
				int var_dimention = delta_compile_line_part(c, line2, 2);
				
				DELTA_WRITE_BYTECODE(BYTECODE_AS1, "", c->total_functions,
									 new_DeltaInstruction3(NULL, BYTECODE_AS1, var_dest,
														   var_dimention, var_id1));
			} else {
				var_dest = delta_get_variable_id(c, c->total_functions, tokens[highest_op_pos - 1]);
				if(var_dest < 0) {
					fprintf(stderr, "Cannot resolve or write to '%s'\n",
							tokens[highest_op_pos - 1]);
					exit(1);
				}
				
				DELTA_WRITE_BYTECODE(BYTECODE_SET, "", c->total_functions,
									 new_DeltaInstruction2(NULL, BYTECODE_SET, var_dest, var_id1));
			}
			result_register = var_dest;
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
		   !strcmp(tokens[highest_op_pos], "!=") ||
		   !strcmp(tokens[highest_op_pos], "<>") ||
		   !strcmp(tokens[highest_op_pos], DELTA_CONCAT_OPERATOR) ||
		   !strcmp(tokens[highest_op_pos], "&&") ||
		   !strcmp(tokens[highest_op_pos], "||")) {
			// resolve the address for the left and right
			int var_id1 = delta_get_variable_id(c, c->total_functions, tokens[highest_op_pos - 1]);
			int var_id2 = delta_get_variable_id(c, c->total_functions, tokens[highest_op_pos + 1]);
			int var_dest = ++var_temp;
			
			if(!strcmp(tokens[highest_op_pos], "+"))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_ADD)
			else if(!strcmp(tokens[highest_op_pos], "-"))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_SUB)
			else if(!strcmp(tokens[highest_op_pos], "*"))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_MUL)
			else if(!strcmp(tokens[highest_op_pos], "/"))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_DIV)
			else if(!strcmp(tokens[highest_op_pos], "%"))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_MOD)
			else if(!strcmp(tokens[highest_op_pos], "<"))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CLT)
			else if(!strcmp(tokens[highest_op_pos], ">"))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CGT)
			else if(!strcmp(tokens[highest_op_pos], "<="))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CLE)
			else if(!strcmp(tokens[highest_op_pos], ">="))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CGE)
			else if(!strcmp(tokens[highest_op_pos], "=="))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CEQ)
			else if(!strcmp(tokens[highest_op_pos], "!="))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CNE)
			else if(!strcmp(tokens[highest_op_pos], DELTA_CONCAT_OPERATOR))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_SAP)
			else if(!strcmp(tokens[highest_op_pos], "<>"))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CNE)
			else if(!strcmp(tokens[highest_op_pos], "&&"))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_AND)
			else if(!strcmp(tokens[highest_op_pos], "||"))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_ORR)
			
			result_register = var_dest;
		}
		
		if(!strcmp(tokens[highest_op_pos], "+=") ||
		   !strcmp(tokens[highest_op_pos], "-=") ||
		   !strcmp(tokens[highest_op_pos], "*=") ||
		   !strcmp(tokens[highest_op_pos], "/=") ||
		   !strcmp(tokens[highest_op_pos], "%=") ||
		   !strcmp(tokens[highest_op_pos], ".=") ||
		   !strcmp(tokens[highest_op_pos], "&&=") ||
		   !strcmp(tokens[highest_op_pos], "||=")) {
			// resolve the address for the left and right
			int var_id1 = delta_get_variable_id(c, c->total_functions, tokens[highest_op_pos - 1]);
			int var_id2 = delta_get_variable_id(c, c->total_functions, tokens[highest_op_pos + 1]);
			int var_dest = var_id1;
			
			// var_id1 must resolve
			if(var_id1 < 0) {
				fprintf(stderr, "LHS of %s must be a variable\n", tokens[highest_op_pos]);
				exit(1);
			}
			
			if(!strcmp(tokens[highest_op_pos], "+="))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_ADD)
			else if(!strcmp(tokens[highest_op_pos], "-="))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_SUB)
			else if(!strcmp(tokens[highest_op_pos], "*="))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_MUL)
			else if(!strcmp(tokens[highest_op_pos], "/="))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_DIV)
			else if(!strcmp(tokens[highest_op_pos], "%="))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_MOD)
			else if(!strcmp(tokens[highest_op_pos], "&&="))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_AND)
			else if(!strcmp(tokens[highest_op_pos], "||="))
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_ORR)
				
			result_register = var_dest;
		}
		
		// reduce temp
		tokens[highest_op_pos - 1] = (char*) malloc(6);
		sprintf(tokens[highest_op_pos - 1], "#%d", var_temp);
		
		for(k = 0; k < total_tokens - highest_op_pos - 2; ++k)
			tokens[highest_op_pos + k] = tokens[highest_op_pos + k + 2];
		
		i += 2;
		total_tokens -= 2;
	}
	
	return result_register;
}


int delta_compile_line(struct DeltaCompiler *c, char* line, int length)
{
	// split the line on commas
	int total_parts = 0, i, last = -1;
	int bcount1 = 0; // ()
	int bcount2 = 0; // []
	int bcount3 = 0; // {}
	char **parts = (char**) malloc(64 * sizeof(char*));
	
	for(i = 0; i < length; ++i) {
		if(line[i] == '(')
			++bcount1;
		if(line[i] == ')')
			--bcount1;
		if(line[i] == '[')
			++bcount2;
		if(line[i] == ']')
			--bcount2;
		if(line[i] == '{')
			++bcount3;
		if(line[i] == '}')
			--bcount3;
		
		// TOOD: quotes don't check for \ to escape quotes
		
		// handle single-quotes
		if(line[i] == '\'') {
			++i;
			for(; i < length; ++i) {
				if(line[i] == '\'')
					break;
			}
		}
		
		// handle double-quotes
		if(line[i] == '"') {
			++i;
			for(; i < length; ++i) {
				if(line[i] == '"')
					break;
			}
		}
		
		if((line[i] == ',' && !bcount1 && !bcount2 && !bcount3) || i == length - 1) {
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
		int key_addr, blank_key = 0;
		if(key == NULL) {
			blank_key = 1;
			key = (char*) malloc(8);
			sprintf(key, "%d", i);
			key_addr = delta_push_string_constant(c, c->total_functions, key, 0);
		}
		else
			key_addr = delta_compile_line_part(c, key, strlen(key));
		
		arg_ptr[arg_depth - 1][(i * 2) + 1] = key_addr;
		
		if(blank_key) {
			// evaluate whole code.
			arg_ptr[arg_depth - 1][(i * 2) + 2] =
				delta_compile_line_part(c, parts[i], strlen(parts[i]));
		} else {
			// only evaluate the code after the '=>' operator.
			arg_ptr[arg_depth - 1][(i * 2) + 2] =
				delta_compile_line_part(c, parts[i] + strlen(key) + 2,
										strlen(parts[i]) - strlen(key) - 2);
		}
		
		--arg_depth;
	}
	arg_ptr[arg_depth][0] = var_temp++;
	
	return arg_ptr[arg_depth][0];
}


int delta_compile_block(struct DeltaCompiler *c, char *identifier, char *block, int at, int end)
{
	// prepare
	char* line = (char*) malloc(1024);
	int i, line_pos = 0, function_id = c->total_functions, identifier_len = 0;
	char **split_semi = NULL; // this is used for 'for' statements
	char *short_identifier = "";
	int full_break = 0;
	
	// qualifiers
	int isAbstract = 0, isStatic = 0, permissionLevel = DELTA_PUBLIC;
	
	// read identifier words
	for(i = 0; i < strlen(identifier); ) {
		delta_skip_spaces(identifier, &i);
		int ii = i;
		for(; i < strlen(identifier); ++i) {
			if(!isalnum(identifier[i])) {
				//if(short_identifier != NULL)
				//	free(short_identifier);
				short_identifier = delta_copy_substring(identifier, ii, i);
				identifier_len = ii - i;
				
				if(!strcmp(short_identifier, "abstract"))
					isAbstract = 1;
				else if(!strcmp(short_identifier, "class"))
					full_break = 1;
				else if(!strcmp(short_identifier, "function"))
					full_break = 1;
				else if(!strcmp(short_identifier, "public"))
					permissionLevel = DELTA_PUBLIC;
				else if(!strcmp(short_identifier, "protected"))
					permissionLevel = DELTA_PROTECTED;
				else if(!strcmp(short_identifier, "private"))
					permissionLevel = DELTA_PRIVATE;
				else if(!strcmp(short_identifier, "static"))
					isStatic = 1;
				else {
					char msg[256];
					sprintf(msg, "Unknown qualifier '%s'", short_identifier);
					delta_error_push(c, line_number, msg);
				}
				
				break;
			}
		}
		
		if(full_break)
			break;
	}
	
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
		
		// convert the expression to a safe BOOLEAN
		++var_temp;
		DELTA_WRITE_BYTECODE(BYTECODE_ZBO, "", c->total_functions,
							 new_DeltaInstruction2(NULL, BYTECODE_ZBO, var_temp, expr_eval));
		
		// perform if statement
		++label_id;
		DELTA_WRITE_BYTECODE(BYTECODE_IFS, "", c->total_functions,
							 new_DeltaInstruction2(NULL, BYTECODE_IFS, label_id, var_temp));
	}
	else if(!strcmp(short_identifier, "while")) {
		// if statement, get the conditional expression
		int expr_at = delta_strpos(identifier, "(") + 1;
		int expr_end = delta_strrchr(identifier, ')');
		char *expr = (char*) malloc(expr_end - expr_at + 1);
		strncpy(expr, identifier + expr_at, expr_end - expr_at);
		
		// loop label
		DELTA_WRITE_BYTECODE(BYTECODE_LBL, "", c->total_functions,
							 new_DeltaInstruction1(NULL, BYTECODE_LBL, label_id));
		
		// evaluate expression
		int expr_eval = delta_compile_line_part(c, expr, expr_end - expr_at);
		
		// convert the expression to a save BOOLEAN
		++var_temp;
		DELTA_WRITE_BYTECODE(BYTECODE_ZBO, "", c->total_functions,
							 new_DeltaInstruction2(NULL, BYTECODE_ZBO, var_temp, expr_eval));
		
		// perform if statement
		++label_id;
		DELTA_WRITE_BYTECODE(BYTECODE_LOP, "", c->total_functions,
							 new_DeltaInstruction2(NULL, BYTECODE_LOP, label_id, var_temp));
	}
	else if(!strcmp(short_identifier, "for")) {
		// for statements work basically the same way as while statements
		
		// get the expression
		int expr_at = delta_strpos(identifier, "(") + 1;
		int expr_end = delta_strrchr(identifier, ')');
		char *expr = (char*) malloc(expr_end - expr_at + 1);
		strncpy(expr, identifier + expr_at, expr_end - expr_at);
		
		// split up the expression on the ';'
		split_semi = delta_split_semicolons(expr);
		
		// compile the 'before', we dont need the return register because its a stand alone line
		// that doesn't have anything to do with the loop iterations
		delta_compile_line_part(c, split_semi[0], strlen(split_semi[0]));
		
		// loop label
		DELTA_WRITE_BYTECODE(BYTECODE_LBL, "", c->total_functions,
							 new_DeltaInstruction1(NULL, BYTECODE_LBL, label_id));
		
		// evaluate expression
		int expr_eval = delta_compile_line_part(c, split_semi[1], strlen(split_semi[1]));
		
		// convert the expression to a save BOOLEAN
		++var_temp;
		DELTA_WRITE_BYTECODE(BYTECODE_ZBO, "", c->total_functions,
							 new_DeltaInstruction2(NULL, BYTECODE_ZBO, var_temp, expr_eval));
		
		// perform if statement
		++label_id;
		DELTA_WRITE_BYTECODE(BYTECODE_LOP, "", c->total_functions,
							 new_DeltaInstruction2(NULL, BYTECODE_LOP, label_id, var_temp));
	}
	else if(!strcmp(short_identifier, "function")) {
		// when we encounter a function we save the pointer of the block and compile it later
		// this isn't what you'd expect but I feel it will make nested functions in the future
		// easier to handle
		identifier = delta_trim(identifier + delta_strpos(identifier, "function") + 8);
		if(class_name != NULL) {
			char* buf = (char*) malloc(256);
			sprintf(buf, "%s.%s", class_name, identifier);
			identifier = buf;
		}
		
		++c->total_functions;
		c->functions[c->total_functions].name = identifier;
		c->functions[c->total_functions].jit_ptr = NULL;
		c->functions[c->total_functions].is_static = isStatic;
		c->functions[c->total_functions].permission = permissionLevel;
		
		delta_bytecode_writer_function(c, identifier);
	}
	else if(!strcmp(short_identifier, "class")) {
		if(class_name != NULL)
			free(class_name);
		
		identifier = delta_trim(identifier + delta_strpos(identifier, "class") + 5);
		class_name = delta_copy_string(identifier);
		
		// the class name must be valid
		if(toupper(class_name[0]) != class_name[0])
			delta_error_push(c, line_number, "Class names must start with a capital letter.");
		
		delta_bytecode_writer_class(c, class_name);
	}
	else {
		printf("Unknown block identifier '%s'", short_identifier);
		exit(0);
	}
	
	// dissect lines
	int br1 = 0; // ()
	int br2 = 0; // []
	for(i = at; i < end; ++i) {
		// line numbers
		if(block[i] == '\n')
			++line_number;
		
		// single line comment
		if(block[i] == '/' && block[i + 1] == '/') {
			for(; i < end; ++i) {
				if(block[i] == '\n') {
					++line_number;
					break;
				}
			}
			++i;
			continue;
		}
		
		// multi line comment
		if(block[i] == '/' && block[i + 1] == '*') {
			++i;
			for(; i < end; ++i) {
				if(block[i] == '*' && block[i + 1] == '/')
					break;
			}
			++i;
			continue;
		}
		
		// TOOD: single and double quotes do not handle the escape character.
		
		// single-quotes
		if(block[i] == '\'') {
			line[line_pos++] = block[i];
			++i;
			for(; i < end; ++i) {
				line[line_pos++] = block[i];
				if(block[i] == '\'')
					break;
			}
			++i;
		}
		
		// double-quotes
		if(block[i] == '\"') {
			line[line_pos++] = block[i];
			++i;
			for(; i < end; ++i) {
				line[line_pos++] = block[i];
				if(block[i] == '\"')
					break;
			}
			++i;
		}
		
		// count brackets
		if(block[i] == '(')
			++br1;
		else if(block[i] == ')')
			--br1;
		else if(block[i] == '[')
			++br2;
		else if(block[i] == ']')
			--br2;
		
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
			
			delta_compile_block(c, new_identifier, block, orig, i);
			line_pos = 0;
			continue;
		}
		
		else if(block[i] == ';' && !br1 && !br2) {
			line[line_pos] = 0;
			
			int temp = c->total_functions;
			c->total_functions = (strcmp(short_identifier, "") ? function_id + 1 : 0);
			delta_compile_line(c, line, line_pos);
			c->total_functions = temp;
			line_pos = 0;
		}
		
		else
			line[line_pos++] = block[i];
	}
	
	// add forward patch
	if(!strcmp(short_identifier, "if")) {
		// add the jump for else
		//printf("BYTECODE_JMP ( %d )\n", label_id + 1);
		//DeltaFunction_push(c, c->total_functions, new_DeltaInstruction1(NULL, BYTECODE_JMP, label_id + 1));
		
		printf("{%d} BYTECODE_PAT ( %d ) <- if\n", c->total_functions, label_id);
		DeltaFunction_push(c, c->total_functions, new_DeltaInstruction1(NULL, BYTECODE_PAT, label_id));
		--label_id;
	}
	else if(!strcmp(short_identifier, "else")) {
		// patch from if jump
		printf("{%d} BYTECODE_PAT ( %d ) <- else\n", c->total_functions, label_id);
		DeltaFunction_push(c, c->total_functions, new_DeltaInstruction1(NULL, BYTECODE_PAT, label_id));
		--label_id;
	}
	else if(!strcmp(short_identifier, "while")) {
		// jump back to the while expression for the next iteration
		printf("{%d} BYTECODE_GTO ( %d )\n", c->total_functions, label_id - 1);
		DeltaFunction_push(c, c->total_functions, new_DeltaInstruction1(NULL, BYTECODE_GTO, label_id - 1));
		
		// patch the forward jump when the loops ends
		printf("{%d} BYTECODE_PAT ( %d ) <- while\n", c->total_functions, label_id);
		DeltaFunction_push(c, c->total_functions, new_DeltaInstruction1(NULL, BYTECODE_PAT, label_id));
		--label_id;
	}
	else if(!strcmp(short_identifier, "for")) {
		assert(split_semi != NULL);
		
		// the only thing different between the end of a while loop and a for loop is that we add
		// in the incrementor for for statements
		delta_compile_line_part(c, split_semi[2], strlen(split_semi[2]));
		
		// jump back to the while expression for the next iteration
		printf("{%d} BYTECODE_GTO ( %d )\n", c->total_functions, label_id - 1);
		DeltaFunction_push(c, c->total_functions, new_DeltaInstruction1(NULL, BYTECODE_GTO, label_id - 1));
		
		// patch the forward jump when the loops ends
		printf("{%d} BYTECODE_PAT ( %d ) <- while\n", c->total_functions, label_id);
		DeltaFunction_push(c, c->total_functions, new_DeltaInstruction1(NULL, BYTECODE_PAT, label_id));
		--label_id;
	}
	else if(!strcmp(short_identifier, "class")) {
		delta_bytecode_writer_end_class(c, class_name);
		
		if(class_name != NULL)
			free(class_name);
		class_name = NULL;
	}
	else if(!strcmp(short_identifier, "function")) {
		c->functions[c->total_functions].total_vars = var_temp;
		
		// make sure there is always a return at the end of the function
		DELTA_WRITE_UNLINED_BYTECODE(BYTECODE_RTN, "Always add a return", c->total_functions,
									 new_DeltaInstruction0(NULL, BYTECODE_RTN));
		
		delta_bytecode_writer_end_function(c, "");
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
	int i;
	delta_function_reset();
	delta_compile_block(c, "", whole_file, 0, total_bytes);
	
	// the main function is the function that will not have a name
	++c->total_functions;
	for(i = 0; i < c->total_functions; ++i) {
		if(c->functions[i].name == NULL) {
			c->functions[i].name = DELTA_MAIN_FUNCTION;
			c->functions[i].jit_ptr = NULL;
		}
	}
	
	// optimise bytecode for all functions
	for(i = 0; i < c->total_functions; ++i)
		delta_optimise_bytecode(c, i);
	
	// clean up
	fclose(f);
	free(whole_file);
	return DELTA_SUCCESS;
}


void delta_compiler_defaults(struct DeltaCompiler* c)
{
	c->option_virtual_vm = 0;
}


struct DeltaCompiler* delta_compiler_init()
{	
	struct DeltaCompiler *c = new_DeltaCompiler();
	int i;
	
	// prepare built-in predefined constants
	c->alloc_delta_defines = 200;
	c->total_delta_defines = 0;
	c->delta_defines = (struct DeltaDefine*)
		calloc(c->alloc_delta_defines, sizeof(struct DeltaDefine));
	
	// set defaults
	c->errors = 0;
	delta_compiler_defaults(c);
	
	return c;
}


int delta_needs_compile(char *file1, char *file2)
{
	struct stat stat1, stat2;
	stat(file1, &stat1);
	stat(file2, &stat2);
	
	return stat1.st_mtime != stat2.st_mtime;
}

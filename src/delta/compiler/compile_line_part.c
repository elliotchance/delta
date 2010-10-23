/*
 *  Delta programming language
 */

#include "compile_line_part.h"
#include "delta/compiler/bytecode.h"
#include "delta/macros.h"
#include "compiler.h"
#include "delta/compiler/strings.h"
#include "delta/compiler/bytecode_writer.h"
#include "delta/compiler/token.h"
#include "delta/compiler/constant.h"
#include "delta/compiler/errors.h"
#include "delta/structs/DeltaInstruction.h"
#include "delta/structs/DeltaFunction.h"
#include "delta/compiler/compile_block.h"
#include <ctype.h>
#include <assert.h>


int delta_compile_line_part(struct DeltaCompiler *c, char* line, int length, int function_id)
{
	if(!strncmp(delta_trim(line), "return", 6)) {
		int pos = delta_strpos(line, "r") + 6;
		int var_id1 = delta_compile_line_part(c, line + pos, length - pos, function_id);
		printf("var_id1 = %d\n", var_id1);
		
		DELTA_WRITE_BYTECODE(BYTECODE_SET, "", function_id,
							 new_DeltaInstruction2(NULL, BYTECODE_SET, RETURN_REGISTER, var_id1));
		DELTA_WRITE_BYTECODE(BYTECODE_RTN, "", function_id,
							 new_DeltaInstruction0(NULL, BYTECODE_RTN));
		
		return var_id1;
	}
	
	if(!strncmp(delta_trim(line), "var", 3)) {
		// declare the variable
		line = delta_trim(line);
		line = delta_copy_substring(line, 4, strlen(line));
		
		// make sure the variable name is valid
		if(tolower(line[0]) != line[0])
			delta_error_push(c, line_number,
							 "Variables names must start with a non-capital letter.");
		
		int total_vars = c->functions[function_id].total_vars;
		c->functions[function_id].vars[total_vars].type = DELTA_TYPE_NUMBER;
		c->functions[function_id].vars[total_vars].name = (char*) malloc(strlen(line) + 1);
		strcpy(c->functions[function_id].vars[total_vars].name, line);
		c->functions[function_id].vars[total_vars].ram_location = ++var_temp;
		printf("%s defined in %s\n", c->functions[function_id].vars[total_vars].name, c->functions[function_id].name);
		++c->functions[function_id].total_vars;
		
		return -1;
	}
	
	if(!strncmp(delta_trim(line), "public", 6)) {
		delta_compile_block(c, line, "", 0, strlen(line));
		return -1;
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
		
		token = delta_read_token(c, function_id, line, &i);
		token = delta_replace_constant(c, token);
		if(strlen(token) > 0)
			tokens[total_tokens++] = token;
		
		if(delta_is_number(token)) {
			++var_temp;
			sprintf(token, "#%d", delta_push_number_constant(c, function_id, atof(token)));
		}
		else if(delta_is_string(token)) {
			if(delta_is_magic_string(token)) {
				token = delta_translate_magic_string(token);
				//printf("token = '%s'\n", token);
				int res_reg = delta_compile_line_part(c, token, strlen(token), function_id);
				sprintf(token, "#%d", res_reg);
			} else {
				int escape = (token[0] == '"');
				char *str = delta_copy_substring(token, 1, strlen(token) - 1);
				sprintf(token, "#%d", delta_push_constant(c, function_id, str, escape));
			}
		}
		else if(!stricmp(token, "true")) {
			++var_temp;
			sprintf(token, "#%d", delta_push_boolean_constant(c, function_id, DELTA_TRUE));
		}
		else if(!stricmp(token, "false")) {
			++var_temp;
			sprintf(token, "#%d", delta_push_boolean_constant(c, function_id, DELTA_FALSE));
		}
		else if(!stricmp(token, "null")) {
			++var_temp;
			sprintf(token, "#%d", delta_push_null_constant(c, function_id));
		}
		else if(delta_is_keyword(token)) {
			// fine
		}
		else if(token[0] == '#') {
			// fine
		}
		else if(!delta_is_declared(c, function_id, token)) {
			// look for member variable
			int pos = delta_strpos(token, ".");
			if(pos >= 0) {
				// turn the member variable name into a constant string
				int var_dest = ++var_temp;
				char *member_var = delta_copy_substring(token, pos + 1, strlen(token));
				int var_arg0 = delta_push_string_constant(c, function_id, member_var, 0);
			}
			else {
				// cannot be found
				char* buf = (char*) malloc(256);
				sprintf(buf, "Variable %s is not declared.", token);
				delta_error_push(c, line_number, buf);
			}
		}
		
		// skip any spaces before inverse token (aka operator)
		delta_skip_spaces(line, &i);
		
		token = delta_read_inv_token(line, &i);
		if(strlen(token) > 0)
			tokens[total_tokens++] = token;
	}
	
	if(total_tokens == 0)
		return var_temp;
	else if(total_tokens == 1)
		return delta_get_variable_id(c, function_id, tokens[0], NULL);
	
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
			int var_id1 = delta_get_variable_id(c, function_id, tokens[highest_op_pos + 1],
												tokens[highest_op_pos]);
			assert(var_id1 >= 0);
			
			int pos = delta_strpos(tokens[highest_op_pos - 1], "[");
			int pos2 = delta_strpos(tokens[highest_op_pos - 1], ".");
			if(pos > 0) {
				char *rawname = delta_copy_substring(tokens[highest_op_pos - 1], 0, pos);
				var_dest = delta_get_variable_id(c, function_id, rawname,
												 tokens[highest_op_pos]);
				
				if(var_dest < 0) {
					fprintf(stderr, "Cannot resolve (ref1) or write to '%s'\n", rawname);
					exit(1);
				}
				
				char *line2 = delta_copy_substring(tokens[highest_op_pos - 1], pos + 1,
												   strlen(tokens[highest_op_pos - 1]) - 1);
				int var_dimention = delta_compile_line_part(c, line2, strlen(line2), function_id);
				
				DELTA_WRITE_BYTECODE(BYTECODE_AS1, "", function_id,
									 new_DeltaInstruction3(NULL, BYTECODE_AS1, var_dest,
														   var_dimention, var_id1));
			}
			else if(pos2 > 0) {
				var_dest = delta_get_variable_id(c, function_id, tokens[highest_op_pos - 1],
												 tokens[highest_op_pos]);
				
				if(var_dest < 0) {
					fprintf(stderr, "Cannot resolve (ref2) or write to '%s'\n",
							tokens[highest_op_pos - 1]);
					exit(1);
				}
				
				char *line2 = delta_copy_substring(tokens[highest_op_pos - 1], pos2 + 1,
												   strlen(tokens[highest_op_pos - 1]));
				int var_dimention = delta_push_string_constant(c, function_id, line2, 0);
				
				DELTA_WRITE_BYTECODE(BYTECODE_OST, "", function_id,
									 new_DeltaInstruction3(NULL, BYTECODE_OST, var_dest,
														   var_dimention, var_id1));
			}
			else {
				var_dest = delta_get_variable_id(c, function_id, tokens[highest_op_pos - 1],
												 tokens[highest_op_pos]);
				//assert(var_dest >= 0);
				if(var_dest < 0) {
					fprintf(stderr, "Cannot resolve (ref3) or write to '%s'\n",
							tokens[highest_op_pos - 1]);
					exit(1);
				}
				
				DELTA_WRITE_BYTECODE(BYTECODE_SET, "", function_id,
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
			int var_id1 = delta_get_variable_id(c, function_id, tokens[highest_op_pos - 1],
												tokens[highest_op_pos]);
			assert(var_id1 >= 0);
			int var_id2 = delta_get_variable_id(c, function_id, tokens[highest_op_pos + 1],
												tokens[highest_op_pos]);
			assert(var_id2 >= 0);
			int var_dest = ++var_temp;
			
			if(!strcmp(tokens[highest_op_pos], "+")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_ADD)
			}
			else if(!strcmp(tokens[highest_op_pos], "-")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_SUB)
			}
			else if(!strcmp(tokens[highest_op_pos], "*")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_MUL)
			}
			else if(!strcmp(tokens[highest_op_pos], "/")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_DIV)
			}
			else if(!strcmp(tokens[highest_op_pos], "%")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_MOD)
			}
			else if(!strcmp(tokens[highest_op_pos], "<")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CLT)
			}
			else if(!strcmp(tokens[highest_op_pos], ">")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CGT)
			}
			else if(!strcmp(tokens[highest_op_pos], "<=")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CLE)
			}
			else if(!strcmp(tokens[highest_op_pos], ">=")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CGE)
			}
			else if(!strcmp(tokens[highest_op_pos], "==")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CEQ)
			}
			else if(!strcmp(tokens[highest_op_pos], "!=")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CNE)
			}
			else if(!strcmp(tokens[highest_op_pos], DELTA_CONCAT_OPERATOR)) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_SAP)
			}
			else if(!strcmp(tokens[highest_op_pos], "<>")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_CNE)
			}
			else if(!strcmp(tokens[highest_op_pos], "&&")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_AND)
			}
			else if(!strcmp(tokens[highest_op_pos], "||")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_ORR)
			}
			
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
			int var_id1 = delta_get_variable_id(c, function_id, tokens[highest_op_pos - 1],
												tokens[highest_op_pos]);
			assert(var_id1 >= 0);
			int var_id2 = delta_get_variable_id(c, function_id, tokens[highest_op_pos + 1],
												tokens[highest_op_pos]);
			assert(var_id2 >= 0);
			int var_dest = var_id1;
			
			// var_id1 must resolve
			if(var_id1 < 0) {
				fprintf(stderr, "LHS of %s must be a variable\n", tokens[highest_op_pos]);
				exit(1);
			}
			
			if(!strcmp(tokens[highest_op_pos], "+=")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_ADD)
			}
			else if(!strcmp(tokens[highest_op_pos], "-=")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_SUB)
			}
			else if(!strcmp(tokens[highest_op_pos], "*=")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_MUL)
			}
			else if(!strcmp(tokens[highest_op_pos], "/=")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_DIV)
			}
			else if(!strcmp(tokens[highest_op_pos], "%=")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_MOD)
			}
			else if(!strcmp(tokens[highest_op_pos], "&&=")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_AND)
			}
			else if(!strcmp(tokens[highest_op_pos], "||=")) {
				DELTA_ADD_OPERATOR_BYTECODE(BYTECODE_ORR)
			}
			
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

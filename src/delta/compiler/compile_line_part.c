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
#include <ctype.h>


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
			int var_id1 = delta_get_variable_id(c, c->total_functions, tokens[highest_op_pos - 1]);
			int var_id2 = delta_get_variable_id(c, c->total_functions, tokens[highest_op_pos + 1]);
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

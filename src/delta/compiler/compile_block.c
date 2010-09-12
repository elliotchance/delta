/*
 *  Delta programming language
 */

#include "compile_block.h"
#include "delta/macros.h"
#include "delta/compiler/bytecode.h"
#include "delta/compiler/bytecode_writer.h"
#include "delta/compiler/token.h"
#include "delta/compiler/strings.h"
#include "delta/compiler/errors.h"
#include "delta/compiler/constant.h"
#include "compiler.h"
#include "delta/structs/DeltaInstruction.h"
#include "delta/structs/DeltaFunction.h"
#include "delta/compiler/compile_line_part.h"
#include <ctype.h>
#include <assert.h>


int delta_compile_block(struct DeltaCompiler *c, char *identifier, char *block, int at, int end)
{
	// prepare
	char* line = (char*) malloc(1024);
	int i, line_pos = 0, function_id = c->total_functions, identifier_len = 0, this_function_id = 0;
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
		
		// constructors in abstract classes are not allowed
		if(delta_is_constructor(identifier) &&
		   c->classes[delta_get_class_id(c, class_name)].is_abstract)
			delta_error_push(c, line_number, "Abstract classes cannot have constructors.");
		
		this_function_id = ++c->total_functions;
		c->functions[c->total_functions].name = identifier;
		c->functions[c->total_functions].jit_ptr = NULL;
		c->functions[c->total_functions].is_static = isStatic;
		c->functions[c->total_functions].permission = permissionLevel;
		
		// constructors are always static
		if(delta_is_constructor(identifier))
			c->functions[c->total_functions].is_static = DELTA_TRUE;
		
		delta_bytecode_writer_function(c, identifier);
	}
	else if(!strcmp(short_identifier, "class")) {
		if(class_name != NULL)
			free(class_name);
		
		identifier = delta_trim(identifier + delta_strpos(identifier, "class") + 5);
		class_name = delta_copy_string(identifier);
		c->classes[c->total_classes].name = class_name;
		c->classes[c->total_classes].is_abstract = isAbstract;
		++c->total_classes;
		
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
		// if this is a constructor we need to return the new Object
		if(delta_is_constructor(c->functions[this_function_id].name)) {
			int c1 = delta_push_string_constant(c, this_function_id, "0", 0);
			int c2 = delta_push_string_constant(c, this_function_id, "MyObject", 0);
			DELTA_WRITE_UNLINED_BYTECODE(BYTECODE_CAL, "Create the object", this_function_id,
										 new_DeltaInstruction3("new", BYTECODE_CAL, RETURN_REGISTER,
															   c1, c2));
			DELTA_WRITE_UNLINED_BYTECODE(BYTECODE_RTN, "Return this object", this_function_id,
										 new_DeltaInstruction0(NULL, BYTECODE_RTN));
		}
		else {
			// make sure there is always a return at the end of the function
			DELTA_WRITE_UNLINED_BYTECODE(BYTECODE_RTN, "Always add a return", this_function_id,
										 new_DeltaInstruction0(NULL, BYTECODE_RTN));
		}
		
		c->functions[this_function_id].total_vars = var_temp;
		delta_bytecode_writer_end_function(c, "");
	}
	
	// clean up
	free(line);
	return DELTA_SUCCESS;
}

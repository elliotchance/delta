/*
 *  Delta programming language
 */

#include "compile_line.h"
#include "delta/compiler/compiler.h"
#include "delta/compiler/strings.h"
#include "delta/compiler/constant.h"
#include <string.h>


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

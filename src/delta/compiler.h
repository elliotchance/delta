/*
 *  Delta programming language
 */

#ifndef _DELTA_BYTECODE_COMPILER_H
#define _DELTA_BYTECODE_COMPILER_H


#include "structs.h"

DI             new_DeltaInstruction0(char *name, DeltaByteCode bc);
DI             new_DeltaInstruction1(char *name, DeltaByteCode bc, int destination);
DI             new_DeltaInstruction2(char *name, DeltaByteCode bc, int destination, int source1);
DI             new_DeltaInstruction3(char *name, DeltaByteCode bc, int destination, int source1, int source2);
char*          delta_read_token(DeltaCompiler *c, char* line, int* offset);
char*          delta_read_inv_token(char* line, int* offset);
void           delta_skip_spaces(char* line, int* offset);
int            delta_is_keyword(char* word);
int            delta_is_declared(DeltaCompiler *c, char* varname);
int            delta_is_number(char *word);
int            delta_get_variable_id(DeltaCompiler *c, char* name);
int            delta_get_operator_order(char* op);
int            delta_push_label(DeltaCompiler *c, char *name);
void           delta_compile_function(DeltaFunction *df);
void           DeltaFunction_push(DeltaCompiler* c, DI ins);
DeltaCompiler* new_DeltaCompiler();
int            delta_compile_file(DeltaCompiler *c, const char* input_file);
int            delta_compile_block(DeltaCompiler *c, char *identifier, char *block, int start, int end);
int            delta_compile_line(DeltaCompiler *c, char* line, int length);
void           delta_die(const char* msg);
int            delta_is_string(char* test);
char*          delta_copy_string(char* str);
char*          delta_copy_substring(char* str, int start, int length);
DI             new_DeltaInstructionN(char *name, DeltaByteCode bc);
int            delta_strpos(char *haystack, char *needle);
int            delta_push_constant(DeltaCompiler *c, char *token);
char*          delta_extract_argument_key(char *arg);
int            delta_compile_line_part(DeltaCompiler *c, char* line, int length);
char*          delta_replace_constant(char *token);
int            delta_push_number_constant(DeltaCompiler *c, double value);


#endif

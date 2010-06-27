/*
 *  bytecode_compiler.h
 *  jit
 *
 *  Created by Elliot Chance on 25/06/10.
 *  Copyright 2010 Chance MEDIA Pty Ltd. All rights reserved.
 *
 */

#ifndef _DELTA_BYTECODE_COMPILER_H
#define _DELTA_BYTECODE_COMPILER_H


#include "structs.h"

DI             new_DeltaInstruction0(DeltaByteCode bc);
DI             new_DeltaInstruction1(DeltaByteCode bc, int destination);
DI             new_DeltaInstruction2(DeltaByteCode bc, int destination, int source1);
DI             new_DeltaInstruction3(DeltaByteCode bc, int destination, int source1, int source2);
char*          read_token(DeltaCompiler *c, char* line, int* offset);
char*          read_inv_token(char* line, int* offset);
void           skip_spaces(char* line, int* offset);
int            delta_is_keyword(char* word);
int            delta_is_declared(DeltaCompiler *c, char* varname);
int            delta_is_number(char *word);
int            get_variable_id(DeltaCompiler *c, char* name);
int            get_operator_order(char* op);
int            push_label(DeltaCompiler *c, char *name);
DeltaFunction* new_DeltaFunction();
void           delta_compile_function(DeltaFunction *df);
void           DeltaFunction_push(DeltaCompiler* c, DI ins);
DeltaObject*   new_DeltaObject(int total_functions);
DeltaCompiler* new_DeltaCompiler();
int            delta_compile_file(DeltaCompiler *c, const char* input_file);
int            delta_compile_block(DeltaCompiler *c, char* block, int start, int end);
void           delta_compile_line(DeltaCompiler *c, char* line, int length);
void           delta_die(const char* msg);
int            delta_is_string(char* test);
char*          delta_copy_string(char* str);
char*          delta_copy_substring(char* str, int start, int length);
DI             new_DeltaInstructionN(DeltaByteCode bc);


#endif

/*
 *  Delta programming language
 */

#ifndef _DELTA_BYTECODE_COMPILER_H
#define _DELTA_BYTECODE_COMPILER_H


#include "structs.h"

struct DeltaInstruction new_DeltaInstruction0(char *name, DeltaByteCode bc);
struct DeltaInstruction new_DeltaInstruction1(char *name, DeltaByteCode bc, int destination);
struct DeltaInstruction new_DeltaInstruction2(char *name, DeltaByteCode bc, int destination,
											  int source1);
struct DeltaInstruction new_DeltaInstruction3(char *name, DeltaByteCode bc, int destination,
											  int source1, int source2);
char*                   delta_read_token(struct DeltaCompiler *c, char* line, int* offset);
char*                   delta_read_inv_token(char* line, int* offset);
void                    delta_skip_spaces(char* line, int* offset);
int                     delta_is_keyword(char* word);
int                     delta_is_declared(struct DeltaCompiler *c, char* varname);
int                     delta_get_variable_id(struct DeltaCompiler *c, char* name);
int                     delta_get_operator_order(char* op);
int                     delta_push_label(struct DeltaCompiler *c, char *name);
void                    delta_compile_function(struct DeltaFunction *df);
void                    DeltaFunction_push(struct DeltaCompiler* c, struct DeltaInstruction ins);
struct DeltaCompiler*   new_DeltaCompiler();
int                     delta_compile_file(struct DeltaCompiler *c, const char* input_file);
int                     delta_compile_block(struct DeltaCompiler *c, char *identifier, char *block,
											int start, int end);
int                     delta_compile_line(struct DeltaCompiler *c, char* line, int length);
void                    delta_die(const char* msg);
struct DeltaInstruction new_DeltaInstructionN(char *name, DeltaByteCode bc);
int                     delta_push_constant(struct DeltaCompiler *c, char *token);
int                     delta_compile_line_part(struct DeltaCompiler *c, char* line, int length);
char*                   delta_replace_constant(char *token);
int                     delta_push_number_constant(struct DeltaCompiler *c, double value);
struct DeltaFunction*   new_DeltaFunction(char *name,
										  void (*function_ptr)(struct DeltaInstruction *d),
										  int min_args, int max_args);

#endif

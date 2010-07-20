/*
 *  Delta programming language
 */

#ifndef _DELTA_BYTECODE_COMPILER_H
#define _DELTA_BYTECODE_COMPILER_H


#include "delta/structs.h"


extern int var_temp;
extern int *arg_count;
extern int **arg_ptr;
extern int arg_depth;
extern int label_id;


int                     delta_push_label(struct DeltaCompiler *c, char *name);
void                    delta_compile_function(struct DeltaFunction *df);
int                     delta_compile_file(struct DeltaCompiler *c, const char* input_file);
int                     delta_compile_block(struct DeltaCompiler *c, char *identifier, char *block,
											int start, int end);
int                     delta_compile_line(struct DeltaCompiler *c, char* line, int length);
void                    delta_die(const char* msg);
int                     delta_compile_line_part(struct DeltaCompiler *c, char* line, int length);

#endif
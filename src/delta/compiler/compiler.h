/*
 *  Delta programming language
 */

#ifndef _DELTA_COMPILER_H
#define _DELTA_COMPILER_H


#include "delta/structs.h"


#define RETURN_REGISTER -1


extern int var_temp;
extern int *arg_count;
extern int **arg_ptr;
extern int arg_depth;
extern int label_id;
extern int subexpression_depth;
extern int line_number;
extern int result_register;
extern char *class_name;


int                   delta_is_constructor(char *name);
int                   delta_push_label(struct DeltaCompiler *c, char *name);
int                   delta_compile_file(struct DeltaCompiler *c, const char* input_file);
int                   delta_compile_line(struct DeltaCompiler *c, char* line, int length);
void                  delta_die(const char* msg);
void                  delta_function_reset();
struct DeltaCompiler* delta_compiler_init();
int                   delta_needs_compile(char *file1, char *file2);
void                  delta_compiler_defaults(struct DeltaCompiler* c);


#endif

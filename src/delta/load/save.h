/*
 *  Delta programming language
 */

#ifndef _DELTA_SAVE_H
#define _DELTA_SAVE_H


#include <stdio.h>
#include <stdlib.h>
#include "delta/structs.h"


int delta_save_file(struct DeltaCompiler *c, const char* out_file, char* orig);
int delta_write_string(FILE *fp, char *str);
int delta_write_int(FILE *fp, int value);
int delta_write_bytecode(FILE *fp, DeltaByteCode bc);
int delta_write_double(FILE *fp, double value);


#endif

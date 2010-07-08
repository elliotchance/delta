/*
 *  Delta programming language
 */

#include "structs.h"


double                delta_cast_number(int address);
double                delta_cast_number_var(struct DeltaVariable *v);
int                   delta_cast_int(int address);
int                   delta_cast_int_var(struct DeltaVariable *v);
struct DeltaVariable* delta_cast_string(int address, int *free);
struct DeltaVariable* delta_cast_string_var(struct DeltaVariable *v, int *free);
int                   delta_cast_boolean(int address);
int                   delta_cast_boolean_var(struct DeltaVariable *v);

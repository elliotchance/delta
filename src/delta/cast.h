/*
 *  Delta programming language
 */

#include "structs.h"


double                delta_cast_number(int address);
struct DeltaVariable* delta_cast_string(int address, int *free);
double                delta_cast_number_var(struct DeltaVariable *v);

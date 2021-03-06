/*
 *  Delta programming language
 */

#include "delta/structs.h"


double                  delta_cast_number(struct DeltaVariable *v);
int                     delta_cast_int(struct DeltaVariable *v);
struct DeltaVariable*   delta_cast_string(struct DeltaVariable *v, int *free);
int                     delta_cast_boolean(struct DeltaVariable *v);
char*                   delta_cast_new_string(struct DeltaVariable* v, int *size);
struct DeltaVariable*   delta_cast_array(struct DeltaVariable *v, int *release);
char                    delta_cast_char(struct DeltaVariable* v);

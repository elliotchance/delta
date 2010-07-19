/*
 *  Delta programming language
 */

#include "macros.h"


void delta_array_push(struct DeltaArray *array, struct DeltaArrayValue *element);
void delta_array_push_kv(struct DeltaArray *array, char *key, struct DeltaVariable *value);
void delta_array_push_k_null(struct DeltaArray *array, char *key);
void delta_array_push_k_boolean(struct DeltaArray *array, char *key, int value);
void delta_array_push_k_number(struct DeltaArray *array, char *key, double value);
void delta_array_push_k_string(struct DeltaArray *array, char *key, char *value);
void delta_array_push_k_binary_string(struct DeltaArray *array, char *key, char *value, int len);
int  delta_array_key_exists(struct DeltaArray *array, char *key);

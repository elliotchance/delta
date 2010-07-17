/*
 *  Delta programming language
 */

#include "delta/structs.h"


double                  delta_cast_number(struct DeltaVariable *v);
int                     delta_cast_int(struct DeltaVariable *v);
struct DeltaVariable*   delta_cast_string(struct DeltaVariable *v, int *free);
int                     delta_cast_boolean(struct DeltaVariable *v);
char*                   delta_cast_new_string(struct DeltaVariable* v, int *size);


void                    delta_array_push(struct DeltaArray *array, struct DeltaArrayValue *element);
void                    delta_array_push_kv(struct DeltaArray *array, char *key,
											struct DeltaVariable *value);
void                    delta_array_push_k_null(struct DeltaArray *array, char *key);
void                    delta_array_push_k_boolean(struct DeltaArray *array, char *key, int value);
void                    delta_array_push_k_number(struct DeltaArray *array, char *key,
												  double value);
void                    delta_array_push_k_string(struct DeltaArray *array, char *key, char *value);
void                    delta_array_push_k_binary_string(struct DeltaArray *array, char *key,
														 char *value, int len);


struct DeltaArray       delta_new_array();
struct DeltaArrayValue* delta_new_array_element(char *key, struct DeltaVariable *value);
struct DeltaVariable*   delta_new_null();
struct DeltaVariable*   delta_new_boolean(int value);
struct DeltaVariable*   delta_new_number(double value);
struct DeltaVariable*   delta_new_string(char* str);
struct DeltaVariable*   delta_new_binary_string(char* str, int len);

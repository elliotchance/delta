/*
 *  Delta programming language
 */

#include "macros.h"


struct DeltaArray       delta_new_array();
struct DeltaArrayValue* delta_new_array_element(char *key, struct DeltaVariable *value);
struct DeltaVariable*   delta_new_null();
struct DeltaVariable*   delta_new_boolean(int value);
struct DeltaVariable*   delta_new_number(double value);
struct DeltaVariable*   delta_new_string(char* str);
struct DeltaVariable*   delta_new_binary_string(char* str, int len);

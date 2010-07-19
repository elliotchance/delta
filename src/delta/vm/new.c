/*
 *  Delta programming language
 */

#include "new.h"
#include "macros.h"


struct DeltaArray delta_new_array()
{
	struct DeltaArray r;
	r.elements = 0;
	r.head = NULL;
	r.tail = NULL;
	return r;
}


struct DeltaVariable* delta_new_null()
{
	struct DeltaVariable *temp = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable));
	temp->type = DELTA_TYPE_NULL;
	return temp;
}


struct DeltaVariable* delta_new_boolean(int value)
{
	struct DeltaVariable *temp = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable));
	temp->type = DELTA_TYPE_BOOLEAN;
	temp->value.number = (value == 0 ? DELTA_FALSE : DELTA_TRUE);
	return temp;
}


struct DeltaVariable* delta_new_string(char* str)
{
	return delta_new_binary_string(str, strlen(str));
}


struct DeltaVariable* delta_new_binary_string(char* str, int len)
{
	struct DeltaVariable *temp = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable));
	temp->type = DELTA_TYPE_STRING;
	temp->value.ptr = str;
	temp->size = len;
	return temp;
}


struct DeltaVariable* delta_new_number(double value)
{
	struct DeltaVariable *temp = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable));
	temp->type = DELTA_TYPE_NUMBER;
	temp->value.number = value;
	return temp;
}


struct DeltaArrayValue* delta_new_array_element(char *key, struct DeltaVariable *value)
{
	struct DeltaArrayValue *v = (struct DeltaArrayValue*) malloc(sizeof(struct DeltaArrayValue));
	v->key = key;
	v->value = value;
	return v;
}

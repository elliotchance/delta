/*
 *  Delta programming language
 */

#include "array.h"
#include "new.h"
#include <assert.h>


void delta_array_push_k_null(struct DeltaArray *array, char *key)
{
	delta_array_push(array, delta_new_array_element(key, delta_new_null()));
}


void delta_array_push_k_boolean(struct DeltaArray *array, char *key, int value)
{
	delta_array_push(array, delta_new_array_element(key, delta_new_boolean(value)));
}


void delta_array_push_k_string(struct DeltaArray *array, char *key, char *value)
{
	delta_array_push(array, delta_new_array_element(key, delta_new_string(value)));
}


void delta_array_push_k_binary_string(struct DeltaArray *array, char *key, char *value, int len)
{
	delta_array_push(array, delta_new_array_element(key, delta_new_binary_string(value, len)));
}


void delta_array_push_kv(struct DeltaArray *array, char *key, struct DeltaVariable *value)
{
	delta_array_push(array, delta_new_array_element(key, value));
}


void delta_array_push_k_number(struct DeltaArray *array, char *key, double value)
{
	delta_array_push(array, delta_new_array_element(key, delta_new_number(value)));
}


int delta_array_key_exists(struct DeltaArray *array, char *key)
{
	struct DeltaArrayValue *it = array->head;
	int i;
	
	for(i = 0; i < array->elements; ++i, it = it->next) {
		if(!strcmp(it->key, key))
			return DELTA_YES;
	}
	
	return DELTA_NO;
}


void delta_array_push(struct DeltaArray *array, struct DeltaArrayValue *element)
{
	if(delta_array_key_exists(array, element->key)) {
		struct DeltaArrayValue *it = array->head;
		int i;
		
		for(i = 0; i < array->elements; ++i, it = it->next) {
			if(!strcmp(it->key, element->key)) {
				it->value = element->value;
				break;
			}
		}
		
		return;
	}
	
	if(array->head == NULL) {
		array->head = element;
		array->tail = element;
	}
	else {
		assert(array->tail != NULL);
		array->tail->next = element;
		array->tail = element;
	}
	
	++array->elements;
}

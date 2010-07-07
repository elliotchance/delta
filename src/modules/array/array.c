/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <string.h>


/**
 * @brief Create an array.
 */
DELTA_FUNCTION(array)
{
	ram[DELTA_DEST]->type = DELTA_TYPE_ARRAY;
	ram[DELTA_DEST]->value.array.elements = DELTA_ARGS;
	ram[DELTA_DEST]->value.array.head = NULL;
	ram[DELTA_DEST]->value.array.tail = NULL;
	
	// push the elements
	int i;
	for(i = 0; i < DELTA_ARGS; ++i) {
		// create the array element
		struct DeltaArrayValue *dav = (struct DeltaArrayValue*) malloc(sizeof(struct DeltaArrayValue));
		int release;
		
		struct DeltaVariable *v = delta_cast_string(DELTA_ARG_NAME(i), &release);
		dav->key = (char*) malloc(v->size + 1);
		strncpy(dav->key, v->value.ptr, v->size);
		dav->value = ram[DELTA_ARG(i)];
		
		// push the element onto the tail
		if(ram[DELTA_DEST]->value.array.head == NULL) {
			ram[DELTA_DEST]->value.array.head = dav;
			ram[DELTA_DEST]->value.array.tail = dav;
		}
		
		ram[DELTA_DEST]->value.array.tail->next = dav;
		ram[DELTA_DEST]->value.array.tail = dav;
		
		// clean up
		if(release)
			free(v);
	}
}

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
	DELTA_DEST->type = DELTA_TYPE_ARRAY;
	DELTA_DEST->value.array.elements = DELTA_ARGS;
	DELTA_DEST->value.array.head = NULL;
	DELTA_DEST->value.array.tail = NULL;
	
	// push the elements
	int i;
	for(i = 0; i < DELTA_ARGS; ++i) {
		// create the array element
		struct DeltaArrayValue *dav = (struct DeltaArrayValue*) malloc(sizeof(struct DeltaArrayValue));
		int release;
		
		struct DeltaVariable *v = delta_cast_string(DELTA_ARG_NAME(i), &release);
		dav->key = (char*) malloc(v->size + 1);
		strncpy(dav->key, v->value.ptr, v->size);
		dav->value = DELTA_ARG(i);
		
		// push the element onto the tail
		if(DELTA_DEST->value.array.head == NULL) {
			DELTA_DEST->value.array.head = dav;
			DELTA_DEST->value.array.tail = dav;
		}
		
		DELTA_DEST->value.array.tail->next = dav;
		DELTA_DEST->value.array.tail = dav;
		
		// clean up
		DELTA_RELEASE(release, v);
	}
}

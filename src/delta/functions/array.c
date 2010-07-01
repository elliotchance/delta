/*
 *  Delta programming language
 *
 *  Created by Elliot Chance on 01/07/10.
 */

#include "ins.h"


/**
 * @brief Create an array.
 */
delta_function(array)
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
		dav->key = (char*) malloc(8);
		sprintf(dav->key, "%d", i);
		dav->value = ram[DELTA_ARG(i)];
		
		// push the element onto the tail
		if(ram[DELTA_DEST]->value.array.head == NULL) {
			ram[DELTA_DEST]->value.array.head = dav;
			ram[DELTA_DEST]->value.array.tail = dav;
		}
		
		ram[DELTA_DEST]->value.array.tail->next = dav;
		ram[DELTA_DEST]->value.array.tail = dav;
	}
}

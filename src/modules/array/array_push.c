/*
 *  Delta programming language
 */

#include "ins.h"
#include <string.h>


/**
 * @brief Push an element onto an array.
 * If the type of variable is not an array its value will be taken and converted into the first
 * element of the array resulting in a two element array.
 */
delta_function(array_push)
{
	// first make sure source1 is an array
	if(ram[DELTA_ARG0]->type != DELTA_TYPE_ARRAY) {
		ram[DELTA_ARG0]->type = DELTA_TYPE_ARRAY;
		ram[DELTA_ARG0]->value.array.elements = 0;
		ram[DELTA_ARG0]->value.array.head = NULL;
		ram[DELTA_ARG0]->value.array.tail = NULL;
	}
	
	// create the new element to push
	struct DeltaArrayValue *dav = (struct DeltaArrayValue*) malloc(sizeof(struct DeltaArrayValue));
	dav->key = (char*) malloc(6);
	strcpy(dav->key, "hello");
	//dav->value = (char*) malloc(6);
	//strcpy(dav->value, "world");
	
	// push element
	ram[DELTA_ARG0]->value.array.head = dav;
	ram[DELTA_ARG0]->value.array.tail = dav;
	
	// increment the elements
	++ram[DELTA_ARG0]->value.array.elements;
}

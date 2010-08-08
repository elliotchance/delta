/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @category modules/core/array
 *
 * @brief Pop the element off the end of array.
 * @syntax mixed array_pop ( array &array )
 *
 * array_pop() pops and returns the last value of the array, shortening the array by one element. If
 * array is empty (or is not an array), NULL will be returned. Will additionally produce a Warning
 * when called on a non-array.
 *
 * @example Example #1 array_pop() example
 * stack = array("orange", "banana", "apple", "raspberry");
 * fruit = array_pop(stack);
 * println(fruit);
 * println(stack);
 * @endexample
 *
 * @param array The array to get the value from.
 * @return Returns the last value of array. If array is empty (or is not an array), NULL will be
 *         returned.
 */
DELTA_FUNCTION(array_pop)
{
	// throw a warning and return NULL if we didn't get an array
	if(DELTA_ARG0->type != DELTA_TYPE_ARRAY) {
		DELTA_TRIGGER_ERROR("Value passed to array_pop() is not an array.", DELTA_ERROR_WARNING);
		DELTA_RETURN_NULL;
	}
	
	// must have a size at least 1
	int elements = DELTA_ARG0->value.array.elements;
	if(elements < 1) {
		DELTA_TRIGGER_ERROR("Value passed to array_pop() has no elements.", DELTA_ERROR_WARNING);
		DELTA_RETURN_NULL;
	}
	
	// copy the last value out
	struct DeltaVariable *pop = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable));
	DELTA_COPY_VARIABLE(pop, DELTA_ARG0->value.array.tail->value);
	
	// remove the last element
	int i;
	struct DeltaArrayValue *element;
	DELTA_ENUMERATE_ARRAY(&(DELTA_ARG0->value.array), element, i) {
		if(i == elements - 2)
			DELTA_ARG0->value.array.tail = element;
		
		if(i == elements - 1)
			free(element->key);
	}
	--DELTA_ARG0->value.array.elements;
	
	// return removed value
	DELTA_RETURN_VARIABLE(pop);
}

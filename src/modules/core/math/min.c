/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Find lowest value.
 * @syntax number min ( array values )
 * @syntax number min ( number value1 , number value2 [, number value3... ] )
 *
 * If the first and only parameter is an array, min() returns the lowest value in that array. If at
 * least two parameters are provided, min() returns the smallest of these values.
 *
 * @param values An array containing the values.
 * @return min() returns the numerically lowest of the parameter values.
 * @see max
 * @see count
 */
DELTA_FUNCTION(min)
{
	if(DELTA_ARG0->type == DELTA_TYPE_ARRAY) {
		struct DeltaArray *array = &DELTA_ARG0->value.array;
		struct DeltaArrayValue *e = array->head;
		double lowest = delta_cast_number(e->value);
		e = e->next;
		int i;
		for(i = 1; i < array->elements; ++i, e = e->next) {
			double arg = delta_cast_number(e->value);
			if(arg < lowest)
				lowest = arg;
		}
		
		DELTA_RETURN_NUMBER(lowest);
	}
	
	double lowest = delta_cast_number(DELTA_ARG0);
	int i;
	for(i = 1; i < DELTA_ARGS; ++i) {
		double arg = delta_cast_number(DELTA_ARG(i));
		if(arg < lowest)
			lowest = arg;
	}
	
	DELTA_RETURN_NUMBER(lowest);
}

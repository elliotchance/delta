/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Find highest value.
 * @syntax number max ( array values )
 * @syntax number max ( number value1 , number value2 [, number value3... ] )
 *
 * If the first and only parameter is an array, max() returns the highest value in that array. If at
 * least two parameters are provided, max() returns the biggest of these values.
 *
 * @param values An array containing the values.
 * @return max() returns the numerically highest of the parameter values.
 * @see min
 * @see count
 */
DELTA_FUNCTION(max)
{
	if(DELTA_ARG0->type == DELTA_TYPE_ARRAY) {
		struct DeltaArray *array = &DELTA_ARG0->value.array;
		struct DeltaArrayValue *e = array->head;
		double highest = delta_cast_number(e->value);
		e = e->next;
		int i;
		for(i = 1; i < array->elements; ++i, e = e->next) {
			double arg = delta_cast_number(e->value);
			if(arg > highest)
				highest = arg;
		}
		
		DELTA_RETURN_NUMBER(highest);
	}
	
	double highest = delta_cast_number(DELTA_ARG0);
	int i;
	for(i = 1; i < DELTA_ARGS; ++i) {
		double arg = delta_cast_number(DELTA_ARG(i));
		if(arg > highest)
			highest = arg;
	}
	
	DELTA_RETURN_NUMBER(highest);
}

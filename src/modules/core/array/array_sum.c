/*
 *  Delta programming language
 */

#include "delta/delta.h"


DELTA_FUNCTION(array_sum)
{
	/*double total = 0;
	int i, release;
	struct DeltaVariable *array = delta_cast_array(DELTA_ARG0, &release);
	struct DeltaArrayValue *element;
	
	DELTA_ENUMERATE_ARRAY(&array->value.array, element, i)
		total += delta_cast_number(element->value);
	
	DELTA_RELEASE(release, array);
	DELTA_RETURN_NUMBER(total);*/
	
	double total = 0;
	int i;
	struct DeltaArrayValue *element;
	
	DELTA_ENUMERATE_ARRAY(&DELTA_ARG0->value.array, element, i)
		total += delta_cast_number(element->value);
	
	DELTA_RETURN_NUMBER(total);
}

/*
 *  Delta programming language
 */

#include "delta/vm/cast.h"
#include "delta/structs.h"
#include "delta/vm/vm.h"
#include "delta/macros.h"
#include <string.h>


/**
 * @brief Attempt to cast a variable to a number.
 *
 * When using this function to get the number value of a variable always use the returned value and
 * not reuse the value of the variable you are trying to cast. A cast will only occur when there is
 * no loss is precision or information from the original type, however in some cases a numebr can be
 * derived from a string or other type where its not safe to change it permanently to a number.
 *
 * @param address RAM location of the variable.
 * @return double The value that should be considered the number representation of the variable.
 */
inline double delta_cast_number(struct DeltaVariable *v)
{
	if(v->type == DELTA_TYPE_NUMBER)
		return v->value.number;
	
	if(v->type == DELTA_TYPE_BOOLEAN)
		return (v->value.number ? 1 : 0);
	
	if(v->type == DELTA_TYPE_STRING) {
		// a string can be cast to a number only if there is no loss in precision. we test this by
		// converting the string to a number and then back into a string, if the double converted
		// string is equal to the current value then there is no loss of precision casting.
		char *temp = (char*) malloc(32);
		double temp2 = atof(v->value.ptr);
		sprintf(temp, "%g", temp2);
		
		// check for lost precision
		if(!strcmp(temp, v->value.ptr)) {
			v->type == DELTA_TYPE_NUMBER;
			v->value.number = temp2;
			free(temp);
			return v->value.number;
		}
		
		// just because it could not be cast doesn't mean there is no numerical value to return
		free(temp);
		return temp2;
	}
	
	if(v->type == DELTA_TYPE_ARRAY) {
		// arrays can never be converted into numbers so we only return the number of elements
		return v->value.array.elements;
	}
	
	// objects, resources and NULL must return 0
	return 0.0;
}


inline int delta_cast_int(struct DeltaVariable *v)
{
	return (int) delta_cast_number(v);
}


/**
 * @brief Attempt to cast a variable to a string.
 * 
 * Variables will only be cast when there is no preicion lost. The reason this function returns a
 * DeltaVariable* instead of a char* is because strings in Delta are NUL safe meaning a string can
 * contain binary and even NUL characters. The only way this can work is if a string length is also
 * provided.
 *
 * @param address RAM location of the variable.
 * @param release This will be set to 0 or 1. If set to 1 then the returned DeltaVariable* must be
 *        have free() invoked when you are finished with it to prevent memory leaks.
 */
inline struct DeltaVariable* delta_cast_string(struct DeltaVariable* v, int *release)
{
	if(v->type == DELTA_TYPE_STRING) {
		*release = DELTA_NO;
		return v;
	}
	
	if(v->type == DELTA_TYPE_NULL) {
		struct DeltaVariable *r = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable*));
		r->type = DELTA_TYPE_STRING;
		r->value.ptr = NULL;
		r->size = 0;
		
		*release = DELTA_YES;
		return r;
	}
	
	if(v->type == DELTA_TYPE_BOOLEAN) {
		// convert boolean to string
		struct DeltaVariable *r = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable*));
		r->type = DELTA_TYPE_STRING;
		r->value.ptr = (char*) malloc(2);
		sprintf(r->value.ptr, "%s", (v->value.number ? "1" : ""));
		r->size = strlen(v->value.ptr);
		
		*release = DELTA_YES;
		return r;
	}
	
	if(v->type == DELTA_TYPE_NUMBER) {
		// numbers are always cast into a string
		v->type = DELTA_TYPE_STRING;
		double old_value = v->value.number;
		v->value.ptr = (char*) malloc(24);
		sprintf(v->value.ptr, "%g", old_value);
		v->size = strlen(v->value.ptr);
		
		*release = DELTA_NO;
		return v;
	}
	
	if(v->type == DELTA_TYPE_ARRAY) {
		// arrays can never be cast
		struct DeltaVariable *r = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable*));
		r->type = DELTA_TYPE_STRING;
		r->value.ptr = (char*) malloc(6);
		strcpy(r->value.ptr, "Array");
		r->size = strlen(v->value.ptr);
		
		*release = DELTA_YES;
		return r;
	}
	
	if(v->type == DELTA_TYPE_OBJECT) {
		// objects can never be cast
		struct DeltaVariable *r = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable*));
		r->type = DELTA_TYPE_STRING;
		r->value.ptr = (char*) malloc(9);
		strcpy(r->value.ptr, "(object)");
		r->size = strlen(v->value.ptr);
		
		*release = DELTA_YES;
		return r;
	}
	
	if(v->type == DELTA_TYPE_RESOURCE) {
		// resources can never be cast
		struct DeltaVariable *r = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable*));
		r->type = DELTA_TYPE_STRING;
		r->value.ptr = (char*) malloc(11);
		strcpy(r->value.ptr, "(resource)");
		r->size = strlen(v->value.ptr);
		
		*release = DELTA_YES;
		return r;
	}
	
	// hopefully we will never get to here
	return NULL;
}


inline int delta_cast_boolean(struct DeltaVariable *v)
{
	if(v->type == DELTA_TYPE_BOOLEAN)
		return v->value.number;
	
	// other types can never be cast to a boolean so all other types return an approximation
	if(v->type == DELTA_TYPE_NULL)
		return DELTA_FALSE;
	
	if(v->type == DELTA_TYPE_NUMBER)
		return (v->value.number ? DELTA_TRUE : DELTA_FALSE);
	
	if(v->type == DELTA_TYPE_STRING)
		return (v->size ? DELTA_TRUE : DELTA_FALSE);
	
	if(v->type == DELTA_TYPE_ARRAY)
		return (v->value.array.elements ? DELTA_TRUE : DELTA_FALSE);
	
	if(v->type == DELTA_TYPE_OBJECT)
		return DELTA_FALSE;
	
	if(v->type == DELTA_TYPE_RESOURCE)
		return (v->value.resource.ptr == NULL ? DELTA_FALSE : DELTA_TRUE);
	
	// hopefully we will never get to here
	return DELTA_FALSE;
}

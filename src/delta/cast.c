/*
 *  Delta programming language
 */

#include "cast.h"
#include "structs.h"
#include "vm.h"
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
inline double delta_cast_number(int address)
{
	if(ram[address]->type == DELTA_TYPE_NUMBER)
		return ram[address]->value.number;
	
	if(ram[address]->type == DELTA_TYPE_NULL) {
		ram[address]->type == DELTA_TYPE_NUMBER;
		ram[address]->value.number = 0.0;
		return 0.0;
	}
	
	if(ram[address]->type == DELTA_TYPE_STRING) {
		// TODO: do not cast if there is a loss of precision
		ram[address]->type == DELTA_TYPE_NUMBER;
		ram[address]->value.number = atof(ram[address]->value.ptr);
		return ram[address]->value.number;
	}
	
	if(ram[address]->type == DELTA_TYPE_ARRAY) {
		// arrays can never be converted into numbers so we only return the number of elements
		return ram[address]->value.array.elements;
	}
	
	// we should never get to this point but just in case
	return 0.0;
}


inline double delta_cast_number_var(struct DeltaVariable *v)
{
	if(v->type == DELTA_TYPE_NUMBER)
		return v->value.number;
	
	if(v->type == DELTA_TYPE_NULL) {
		v->type == DELTA_TYPE_NUMBER;
		v->value.number = 0.0;
		return 0.0;
	}
	
	if(v->type == DELTA_TYPE_STRING) {
		// TODO: do not cast if there is a loss of precision
		v->type == DELTA_TYPE_NUMBER;
		v->value.number = atof(v->value.ptr);
		return v->value.number;
	}
	
	if(v->type == DELTA_TYPE_ARRAY) {
		// arrays can never be converted into numbers so we only return the number of elements
		return v->value.array.elements;
	}
	
	// we should never get to this point but just in case
	return 0.0;
}


/**
 * @brief Attemp to cast a variable to a string.
 * 
 * Variables will only be cast when there is no preicion lost.
 *
 * @param address RAM location of the variable.
 */
inline struct DeltaVariable* delta_cast_string(int address, int *free)
{
	if(ram[address]->type == DELTA_TYPE_STRING) {
		*free = DELTA_NO;
		return ram[address];
	}
	
	if(ram[address]->type == DELTA_TYPE_NUMBER) {
		// convert floating point to string
		ram[address]->type = DELTA_TYPE_STRING;
		ram[address]->value.ptr = (char*) malloc(16);
		sprintf(ram[address]->value.ptr, "%g", ram[address]->value.number);
		ram[address]->size = strlen(ram[address]->value.ptr);
		
		*free = DELTA_NO;
		return ram[address];
	}
	
	// hopefully we will never get to here
	return NULL;
}

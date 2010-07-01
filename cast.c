/*
 *  Delta programming language
 *
 *  Created by Elliot Chance on 01/07/10.
 */

#include "cast.h"
#include "structs.h"
#include "vm.h"


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


/**
 * @brief Attemp to convert a variable to a string.
 * 
 * Variables will only be cast when there is no preicion lost.
 *
 * @param address RAM location of the variable.
 * @return int Either DELTA_SUCCESS or DELTA_FAILURE;
 */
inline int delta_cast_string(int address)
{
	if(ram[address]->type == DELTA_TYPE_STRING)
		return DELTA_SUCCESS;
	
	// hopefully we will never get to here
	return DELTA_FAILURE;
}

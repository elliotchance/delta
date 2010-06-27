/*
 *  ins.c
 *  jit
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#include "ins.h"
#include "vm.h"
#include <math.h>


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
	if(ram[address].type == DELTA_TYPE_NUMBER)
		return ram[address].value.number;
	
	if(ram[address].type == DELTA_TYPE_NULL) {
		ram[address].type == DELTA_TYPE_NUMBER;
		ram[address].value.number = 0.0;
		return 0.0;
	}
	
	if(ram[address].type == DELTA_TYPE_STRING) {
		ram[address].type == DELTA_TYPE_NUMBER;
		ram[address].value.number = atof(ram[address].value.ptr);
		return ram[address].value.number;
	}
	
	// we should never get to this point but just in case
	return 0.0;
}


/**
 * @brief Numerical addition.
 */
ins(ADD)
{
	// this is only numerical addition, so other types need to be cast to a number
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	ram[d->destination].value.number = delta_cast_number(d->source1) + delta_cast_number(d->source2);
}


/**
 * @brief Equal to.
 */
ins(CEQ)
{
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	if(delta_cast_number(d->source1) == delta_cast_number(d->source2))
		ram[d->destination].value.number = 1;
	else
		ram[d->destination].value.number = 0;
}


/**
 * @brief Greater than or equal to.
 */
ins(CGE)
{
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	if(delta_cast_number(d->source1) >= delta_cast_number(d->source2))
		ram[d->destination].value.number = 1;
	else
		ram[d->destination].value.number = 0;
}


/**
 * @brief Greater then.
 */
ins(CGT)
{
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	if(delta_cast_number(d->source1) > delta_cast_number(d->source2))
		ram[d->destination].value.number = 1;
	else
		ram[d->destination].value.number = 0;
}


/**
 * @brief Less than or equal to.
 */
ins(CLE)
{
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	if(delta_cast_number(d->source1) <= delta_cast_number(d->source2))
		ram[d->destination].value.number = 1;
	else
		ram[d->destination].value.number = 0;
}


/**
 * @brief Less than.
 */
ins(CLT)
{
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	if(delta_cast_number(d->source1) < delta_cast_number(d->source2))
		ram[d->destination].value.number = 1;
	else
		ram[d->destination].value.number = 0;
}


/**
 * @brief Not equal to.
 */
ins(CNE)
{
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	if(delta_cast_number(d->source1) != delta_cast_number(d->source2))
		ram[d->destination].value.number = 1;
	else
		ram[d->destination].value.number = 0;
}


/**
 * @brief Decrement (subtract 1)
 */
ins(DEC)
{
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	ram[d->destination].value.number = delta_cast_number(d->destination) - 1;
}


/**
 * @brief Numerical division.
 */
ins(DIV)
{
	// this is only numerical division, so other types need to be cast to a number
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	ram[d->destination].value.number = delta_cast_number(d->source1) / delta_cast_number(d->source2);
}


/**
 * @brief Goto label.
 */
ins(GTO)
{
	stack_pos = d->destination;
}


/**
 * @brief if statement.
 * This is translated into jit so this function should never be called, if it is it will do nothing.
 */
ins(IFS)
{
	// done in jit
}


/**
 * @brief Increment (add 1)
 */
ins(INC)
{
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	ram[d->destination].value.number = delta_cast_number(d->destination) + 1;
}


/**
 * @brief Define a label.
 * This bytecode is for marking a label for the jit compiler, this function itself should never be
 * called, however if it does nothing will happen.
 */
ins(LBL)
{
	// this shouldn't even be called but i'll but it in just in case.
}


/**
 * @brief Numerical multiplication.
 */
ins(MUL)
{
	// this is only numerical multiplicaton, so other types need to be cast to a number
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	ram[d->destination].value.number = delta_cast_number(d->source1) * delta_cast_number(d->source2);
}


/**
 * @brief NULL action.
 * This is purposly designed to take no action.
 */
ins(NUL)
{
	// does nothing
}


/**
 * @brief Write a variable to the stdout.
 */
ins(OUT)
{
	if(ram[d->source1].type == DELTA_TYPE_NULL)
		printf("(null)");
	else if(ram[d->source1].type == DELTA_TYPE_NUMBER)
		printf("%g", ram[d->source1].value.number);
	else if(ram[d->source1].type == DELTA_TYPE_STRING)
		printf("%s", ram[d->source1].value.ptr);
	
	// return null
	ram[d->destination].type = DELTA_TYPE_NULL;
	
	printf("\n");
}


/**
 * @brief Return statement.
 */
ins(RTN)
{
	// currently does nothing
}


/**
 * @brief Copy a variable from location to another.
 */
ins(SET)
{
	// this copies the contents of a variable to a new location in RAM
	ram[d->destination].type = ram[d->source1].type;
	ram[d->destination].value.number = ram[d->source1].value.number;
	ram[d->destination].value.ptr = ram[d->source1].value.ptr;
}


/**
 * @brief Square root.
 */
ins(SQT)
{
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	ram[d->destination].value.number = sqrt(delta_cast_number(d->source1));
}


/**
 * @brief Cosine (radians).
 */
ins(COS)
{
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	ram[d->destination].value.number = cos(delta_cast_number(d->source1));
}


/**
 * @brief Sine (radians).
 */
ins(SIN)
{
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	ram[d->destination].value.number = sin(delta_cast_number(d->source1));
}


/**
 * @brief Tangent (radians).
 */
ins(TAN)
{
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	ram[d->destination].value.number = tan(delta_cast_number(d->source1));
}


/**
 * @brief Numerical subtraction.
 */
ins(SUB)
{
	// this is only numerical subtraction, so other types need to be cast to a number
	ram[d->destination].type = DELTA_TYPE_NUMBER;
	ram[d->destination].value.number = delta_cast_number(d->source1) - delta_cast_number(d->source2);
}

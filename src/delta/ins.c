/*
 *  ins.c
 *  jit
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#include "ins.h"
#include "vm.h"
#include <math.h>
#include <string.h>


#define DELTA_ARGS   (d->args - 1)
#define DELTA_DEST   d->arg[0]
#define DELTA_ARG0   d->arg[1]
#define DELTA_ARG1   d->arg[2]
#define DELTA_ARG(i) d->arg[i + 1]

#define print_ins_args(__BYTECODE) \
printf("*BYTECODE_%s (", #__BYTECODE); \
int i; \
for(i = 0; i < DELTA_ARGS + 1; ++i) \
printf(" %d", d->arg[i]); \
printf(" )\n");


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
		// TODO: do not cast if there is a loss of precision
		ram[address].type == DELTA_TYPE_NUMBER;
		ram[address].value.number = atof(ram[address].value.ptr);
		return ram[address].value.number;
	}
	
	if(ram[address].type == DELTA_TYPE_ARRAY) {
		// arrays can never be converted into numbers so we only return the number of elements
		// TODO: get the number of elements
		return 0.0;
	}
	
	// we should never get to this point but just in case
	return 0.0;
}


/**
 * @brief Numerical addition.
 */
ins(ADD)
{
	print_ins_args(ADD);
	
	// this is only numerical addition, so other types need to be cast to a number
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	ram[DELTA_DEST].value.number = delta_cast_number(DELTA_ARG0) + delta_cast_number(DELTA_ARG1);
}


/**
 * @brief Equal to.
 */
ins(CEQ)
{
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	if(delta_cast_number(DELTA_ARG0) == delta_cast_number(DELTA_ARG1))
		ram[DELTA_DEST].value.number = 1;
	else
		ram[DELTA_DEST].value.number = 0;
}


/**
 * @brief Greater than or equal to.
 */
ins(CGE)
{
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	if(delta_cast_number(DELTA_ARG0) >= delta_cast_number(DELTA_ARG1))
		ram[DELTA_DEST].value.number = 1;
	else
		ram[DELTA_DEST].value.number = 0;
}


/**
 * @brief Greater then.
 */
ins(CGT)
{
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	if(delta_cast_number(DELTA_ARG0) > delta_cast_number(DELTA_ARG1))
		ram[DELTA_DEST].value.number = 1;
	else
		ram[DELTA_DEST].value.number = 0;
}


/**
 * @brief Less than or equal to.
 */
ins(CLE)
{
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	if(delta_cast_number(DELTA_ARG0) <= delta_cast_number(DELTA_ARG1))
		ram[DELTA_DEST].value.number = 1;
	else
		ram[DELTA_DEST].value.number = 0;
}


/**
 * @brief Less than.
 */
ins(CLT)
{
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	if(delta_cast_number(DELTA_ARG0) < delta_cast_number(DELTA_ARG1))
		ram[DELTA_DEST].value.number = 1;
	else
		ram[DELTA_DEST].value.number = 0;
}


/**
 * @brief Not equal to.
 */
ins(CNE)
{
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	if(delta_cast_number(DELTA_ARG0) != delta_cast_number(DELTA_ARG1))
		ram[DELTA_DEST].value.number = 1;
	else
		ram[DELTA_DEST].value.number = 0;
}


/**
 * @brief Decrement (subtract 1)
 */
ins(DEC)
{
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	ram[DELTA_DEST].value.number = delta_cast_number(DELTA_DEST) - 1;
}


/**
 * @brief Numerical division.
 */
ins(DIV)
{
	// this is only numerical division, so other types need to be cast to a number
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	ram[DELTA_DEST].value.number = delta_cast_number(DELTA_ARG0) / delta_cast_number(DELTA_ARG1);
}


/**
 * @brief Goto label.
 */
ins(GTO)
{
	stack_pos = DELTA_DEST;
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
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	ram[DELTA_DEST].value.number = delta_cast_number(DELTA_DEST) + 1;
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
	print_ins_args(MUL);
	// this is only numerical multiplicaton, so other types need to be cast to a number
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	ram[DELTA_DEST].value.number = delta_cast_number(DELTA_ARG0) * delta_cast_number(DELTA_ARG1);
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
	print_ins_args(OUT);
	
	for(i = 0; i < DELTA_ARGS; ++i) {
		//printf("DELTA_ARG(%d) = ", DELTA_ARG(i));
		delta_vm_print_variable(&ram[DELTA_ARG(i)]);
		printf("\n");
	}
	
	// return null
	ram[DELTA_DEST].type = DELTA_TYPE_NULL;
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
	ram[DELTA_DEST].type = ram[DELTA_ARG0].type;
	ram[DELTA_DEST].value.number = ram[DELTA_ARG0].value.number;
	ram[DELTA_DEST].value.ptr = ram[DELTA_ARG0].value.ptr;
}


/**
 * @brief Square root.
 */
ins(SQT)
{
	print_ins_args(SQT);
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	ram[DELTA_DEST].value.number = sqrt(delta_cast_number(DELTA_ARG0));
}


/**
 * @brief Cosine (radians).
 */
ins(COS)
{
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	ram[DELTA_DEST].value.number = cos(delta_cast_number(DELTA_ARG0));
}


/**
 * @brief Sine (radians).
 */
ins(SIN)
{
	print_ins_args(SIN);
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	ram[DELTA_DEST].value.number = sin(delta_cast_number(DELTA_ARG0));
}


/**
 * @brief Tangent (radians).
 */
ins(TAN)
{
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	ram[DELTA_DEST].value.number = tan(delta_cast_number(DELTA_ARG0));
}


/**
 * @brief Numerical subtraction.
 */
ins(SUB)
{
	// this is only numerical subtraction, so other types need to be cast to a number
	ram[DELTA_DEST].type = DELTA_TYPE_NUMBER;
	ram[DELTA_DEST].value.number = delta_cast_number(DELTA_ARG0) - delta_cast_number(DELTA_ARG1);
}


/**
 * @brief Push an element onto an array.
 * If the type of variable is not an array its value will be taken and converted into the first
 * element of the array resulting in a two element array.
 */
ins(APH)
{
	// first make sure source1 is an array
	if(ram[DELTA_ARG0].type != DELTA_TYPE_ARRAY) {
		ram[DELTA_ARG0].type = DELTA_TYPE_ARRAY;
		ram[DELTA_ARG0].value.array.elements = 0;
		ram[DELTA_ARG0].value.array.head = NULL;
		ram[DELTA_ARG0].value.array.tail = NULL;
	}
	
	// create the new element to push
	DeltaArrayValue *dav = (DeltaArrayValue*) malloc(sizeof(DeltaArrayValue));
	dav->key = (char*) malloc(6);
	strcpy(dav->key, "hello");
	dav->value = (char*) malloc(6);
	strcpy(dav->value, "world");
	
	// push element
	ram[DELTA_ARG0].value.array.head = dav;
	ram[DELTA_ARG0].value.array.tail = dav;
	
	// increment the elements
	++ram[DELTA_ARG0].value.array.elements;
}

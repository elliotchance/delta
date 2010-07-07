/*
 *  Delta programming language
 */

#include "ins.h"
#include "vm.h"
#include "../macros.h"
#include "delta/vm/cast.h"
#include <math.h>
#include <string.h>


/**
 * @brief Numerical addition.
 */
DELTA_INS(ADD)
{
	//DELTA_PRINT_INS_ARGS(ADD);
	DELTA_RETURN_NUMBER(delta_cast_number(d->arg[1]) + delta_cast_number(d->arg[2]));
}


/**
 * @brief Equal to.
 */
DELTA_INS(CEQ)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->arg[1]) == delta_cast_number(d->arg[2]));
}


/**
 * @brief Greater than or equal to.
 */
DELTA_INS(CGE)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->arg[1]) >= delta_cast_number(d->arg[2]));
}


/**
 * @brief Greater then.
 */
DELTA_INS(CGT)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->arg[1]) > delta_cast_number(d->arg[2]));
}


/**
 * @brief Less than or equal to.
 */
DELTA_INS(CLE)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->arg[1]) <= delta_cast_number(d->arg[2]));
}


/**
 * @brief Less than.
 */
DELTA_INS(CLT)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->arg[1]) < delta_cast_number(d->arg[2]));
}


/**
 * @brief Not equal to.
 */
DELTA_INS(CNE)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->arg[1]) != delta_cast_number(d->arg[2]));
}


/**
 * @brief Decrement (subtract 1)
 */
DELTA_INS(DEC)
{
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_DEST) - 1);
}


/**
 * @brief Numerical division.
 */
DELTA_INS(DIV)
{
	// this is only numerical division, so other types need to be cast to a number
	DELTA_RETURN_NUMBER(delta_cast_number(d->arg[1]) / delta_cast_number(d->arg[2]));
}


/**
 * @brief Goto label.
 */
DELTA_INS(GTO)
{
	stack_pos = DELTA_DEST;
}


/**
 * @brief if statement.
 * This is translated into jit so this function should never be called, if it is it will do nothing.
 */
DELTA_INS(IFS)
{
	// done in jit
}


/**
 * @brief Increment (add 1)
 */
DELTA_INS(INC)
{
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_DEST) + 1);
}


/**
 * @brief Define a label.
 * This bytecode is for marking a label for the jit compiler, this function itself should never be
 * called, however if it does nothing will happen.
 */
DELTA_INS(LBL)
{
	// this shouldn't even be called but i'll but it in just in case.
}


/**
 * @brief Numerical multiplication.
 */
DELTA_INS(MUL)
{
	// this is only numerical multiplicaton, so other types need to be cast to a number
	DELTA_RETURN_NUMBER(delta_cast_number(d->arg[1]) * delta_cast_number(d->arg[2]));
}


/**
 * @brief NULL action.
 * This is purposly designed to take no action.
 */
DELTA_INS(NUL)
{
	// does nothing
}


/**
 * @brief Return statement.
 */
DELTA_INS(RTN)
{
	// currently does nothing
}


/**
 * @brief Copy a variable from one location to another.
 */
DELTA_INS(SET)
{
	// TODO: assigning any type except object requires a complete recursive copy of the variable.
	ram[DELTA_DEST]->type = ram[d->arg[1]]->type;
	ram[DELTA_DEST]->size = ram[d->arg[1]]->size;
	ram[DELTA_DEST]->value.number = ram[d->arg[1]]->value.number;
	ram[DELTA_DEST]->value.ptr = ram[d->arg[1]]->value.ptr;
	ram[DELTA_DEST]->value.array = ram[d->arg[1]]->value.array;
}


/**
 * @brief Numerical subtraction.
 */
DELTA_INS(SUB)
{
	// this is only numerical subtraction, so other types need to be cast to a number
	DELTA_RETURN_NUMBER(delta_cast_number(d->arg[1]) - delta_cast_number(d->arg[2]));
}


/**
 * @brief Assign the first dimension of an array.
 */
DELTA_INS(AS1)
{
	// FIXME: we must be an array
	
	int release_key;
	struct DeltaVariable *v = delta_cast_string(d->arg[1], &release_key);
	
	// try and find the key
	int i, found = 0;
	struct DeltaArrayValue *element = ram[DELTA_DEST]->value.array.head;
	for(i = 0; i < ram[DELTA_DEST]->value.array.elements; ++i, element = element->next) {
		if(!strcmp(v->value.ptr, element->key)) {
			element->value->type = DELTA_TYPE_NUMBER;
			element->value->value.number = ram[d->arg[2]]->value.number;
			found = 1;
			break;
		}
	}
	
	// if the key doesn't exist then push the element
	if(!found) {
		struct DeltaArrayValue *next = (struct DeltaArrayValue*) malloc(sizeof(struct DeltaArrayValue));
		next->key = v->value.ptr;
		next->value = ram[d->arg[2]];
		ram[DELTA_DEST]->value.array.tail->next = next;
		++ram[DELTA_DEST]->value.array.elements;
	}
	
	// clean up
	if(release_key)
		free(v);
}


/**
 * @brief Get the first dimension of an array.
 */
DELTA_INS(AG1)
{
	// FIXME: we must be an array
	
	int release_key;
	struct DeltaVariable *v = delta_cast_string(d->arg[2], &release_key);
	
	// try and find the key
	int i, found = 0;
	struct DeltaArrayValue *element = ram[d->arg[1]]->value.array.head;
	for(i = 0; i < ram[d->arg[1]]->value.array.elements; ++i, element = element->next) {
		if(!strcmp(v->value.ptr, element->key)) {
			ram[DELTA_DEST]->type = element->value->type;
			ram[DELTA_DEST]->value.number = element->value->value.number;
			found = 1;
			break;
		}
	}
	
	// if the key doesn't exist then return NULL
	if(!found)
		ram[DELTA_DEST]->type = DELTA_TYPE_NULL;
	
	// clean up
	if(release_key)
		free(v);
}

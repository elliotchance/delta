/*
 *  Delta programming language
 */

#include "ins.h"
#include "delta/vm/vm.h"
#include "delta/macros.h"
#include "delta/vm/cast.h"
#include <math.h>
#include <string.h>


/**
 * @brief Boolean AND.
 */
DELTA_INS(AND)
{
	DELTA_RETURN_BOOLEAN(delta_cast_boolean(d->varg[1]) && delta_cast_boolean(d->varg[2]));
}


/**
 * @brief Boolean OR.
 */
DELTA_INS(ORR)
{
	DELTA_RETURN_BOOLEAN(delta_cast_boolean(d->varg[1]) || delta_cast_boolean(d->varg[2]));
}


/**
 * @brief Boolean NOT.
 */
DELTA_INS(NOT)
{
	DELTA_RETURN_BOOLEAN(!delta_cast_boolean(d->varg[1]));
}


/**
 * @brief Numerical addition.
 */
DELTA_INS(ADD)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->varg[1]) + delta_cast_number(d->varg[2]));
}


/**
 * @brief String append.
 */
DELTA_INS(SAP)
{
	// get args
	int release0, release1;
	struct DeltaVariable *arg0 = delta_cast_string(d->varg[1], &release0);
	struct DeltaVariable *arg1 = delta_cast_string(d->varg[2], &release1);
	
	// create the new string
	char *r = (char*) malloc(arg0->size + arg1->size + 1);
	strncpy(r, arg0->value.ptr, arg0->size);
	strncpy(r + arg0->size, arg1->value.ptr, arg1->size);
	r[arg0->size + arg1->size] = 0;
	
	// finish
	DELTA_RELEASE(release0, arg0);
	DELTA_RELEASE(release1, arg1);
	DELTA_RETURN_BINARY_STRING(r, arg0->size + arg1->size);
}


/**
 * @brief Fast math numerical addition.
 */
DELTA_INS(NAD)
{
	DELTA_DEST->value.number = d->varg[1]->value.number + d->varg[2]->value.number;
}


/**
 * @brief Fast math numerical subtraction.
 */
DELTA_INS(NSB)
{
	DELTA_DEST->value.number = d->varg[1]->value.number - d->varg[2]->value.number;
}


/**
 * @brief Fast math multiplication addition.
 */
DELTA_INS(NMU)
{
	DELTA_DEST->value.number = d->varg[1]->value.number * d->varg[2]->value.number;
}


/**
 * @brief Fast math numerical division.
 */
DELTA_INS(NDV)
{
	// TODO: divide by zero, also check DIV
	DELTA_DEST->value.number = d->varg[1]->value.number / d->varg[2]->value.number;
}


/**
 * @brief Fast math numerical modulus.
 */
DELTA_INS(NMD)
{
	DELTA_DEST->value.number = fmod(d->varg[1]->value.number, d->varg[2]->value.number);
}


/**
 * @brief FastMath equal to.
 */
DELTA_INS(NEQ)
{
	DELTA_DEST->value.number = (d->varg[1]->value.number == d->varg[2]->value.number);
}


/**
 * @brief FastMath greater than or equal to.
 */
DELTA_INS(NGE)
{
	DELTA_DEST->value.number = (d->varg[1]->value.number >= d->varg[2]->value.number);
}


/**
 * @brief FastMath greater than.
 */
DELTA_INS(NGT)
{
	DELTA_DEST->value.number = (d->varg[1]->value.number > d->varg[2]->value.number);
}


/**
 * @brief FastMath less than or equal to.
 */
DELTA_INS(NLE)
{
	DELTA_DEST->value.number = (d->varg[1]->value.number <= d->varg[2]->value.number);
}


/**
 * @brief FastMath less than.
 */
DELTA_INS(NLT)
{
	DELTA_DEST->value.number = (d->varg[1]->value.number < d->varg[2]->value.number);
}


/**
 * @brief FastMath not equal to.
 */
DELTA_INS(NNE)
{
	DELTA_DEST->value.number = (d->varg[1]->value.number != d->varg[2]->value.number);
}


/**
 * @brief Equal to.
 */
DELTA_INS(CEQ)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->varg[1]) == delta_cast_number(d->varg[2]));
}


/**
 * @brief Greater than or equal to.
 */
DELTA_INS(CGE)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->varg[1]) >= delta_cast_number(d->varg[2]));
}


/**
 * @brief Greater then.
 */
DELTA_INS(CGT)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->varg[1]) > delta_cast_number(d->varg[2]));
}


/**
 * @brief Less than or equal to.
 */
DELTA_INS(CLE)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->varg[1]) <= delta_cast_number(d->varg[2]));
}


/**
 * @brief Less than.
 */
DELTA_INS(CLT)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->varg[1]) < delta_cast_number(d->varg[2]));
}


/**
 * @brief Not equal to.
 */
DELTA_INS(CNE)
{
	DELTA_RETURN_NUMBER(delta_cast_number(d->varg[1]) != delta_cast_number(d->varg[2]));
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
	DELTA_RETURN_NUMBER(delta_cast_number(d->varg[1]) / delta_cast_number(d->varg[2]));
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
 * @brief FastMath increment (add 1)
 */
DELTA_INS(NIN)
{
	++DELTA_DEST->value.number;
}


/**
 * @brief FastMath decrement (add 1)
 */
DELTA_INS(NDE)
{
	--DELTA_DEST->value.number;
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
	DELTA_RETURN_NUMBER(delta_cast_number(d->varg[1]) * delta_cast_number(d->varg[2]));
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
	DELTA_DEST->type = d->varg[1]->type;
	DELTA_DEST->size = d->varg[1]->size;
	DELTA_DEST->value.number = d->varg[1]->value.number;
	DELTA_DEST->value.ptr = d->varg[1]->value.ptr;
	DELTA_DEST->value.array = d->varg[1]->value.array;
	DELTA_DEST->value.resource = d->varg[1]->value.resource;
}


/**
 * @brief Numerical subtraction.
 */
DELTA_INS(SUB)
{
	// this is only numerical subtraction, so other types need to be cast to a number
	DELTA_RETURN_NUMBER(delta_cast_number(d->varg[1]) - delta_cast_number(d->varg[2]));
}


/**
 * @brief Numerical modulus.
 */
DELTA_INS(MOD)
{
	DELTA_RETURN_NUMBER(fmod(delta_cast_number(d->varg[1]), delta_cast_number(d->varg[2])));
}


/**
 * @brief Assign the first dimension of an array.
 */
DELTA_INS(AS1)
{
	// FIXME: we must be an array
	
	int release_key;
	struct DeltaVariable *v = delta_cast_string(d->varg[1], &release_key);
	
	// try and find the key
	int i, found = 0;
	struct DeltaArrayValue *element = DELTA_DEST->value.array.head;
	for(i = 0; i < DELTA_DEST->value.array.elements; ++i, element = element->next) {
		if(!strcmp(v->value.ptr, element->key)) {
			element->value->type = DELTA_TYPE_NUMBER;
			element->value->value.number = d->varg[2]->value.number;
			found = 1;
			break;
		}
	}
	
	// if the key doesn't exist then push the element
	if(!found) {
		struct DeltaArrayValue *next = (struct DeltaArrayValue*) malloc(sizeof(struct DeltaArrayValue));
		next->key = v->value.ptr;
		next->value = d->varg[2];
		DELTA_DEST->value.array.tail->next = next;
		++DELTA_DEST->value.array.elements;
	}
	
	// clean up
	DELTA_RELEASE(release_key, v);
}


/**
 * @brief Get the first dimension of an array.
 */
DELTA_INS(AG1)
{
	// FIXME: we must be an array
	
	int release_key;
	struct DeltaVariable *v = delta_cast_string(d->varg[2], &release_key);
	
	// try and find the key
	int i, found = 0;
	struct DeltaArrayValue *element = d->varg[1]->value.array.head;
	for(i = 0; i < d->varg[1]->value.array.elements; ++i, element = element->next) {
		if(!strcmp(v->value.ptr, element->key)) {
			DELTA_DEST->type = element->value->type;
			DELTA_DEST->value.number = element->value->value.number;
			found = 1;
			break;
		}
	}
	
	// if the key doesn't exist then return NULL
	if(!found)
		DELTA_DEST->type = DELTA_TYPE_NULL;
	
	// clean up
	DELTA_RELEASE(release_key, v);
}

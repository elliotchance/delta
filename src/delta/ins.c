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


/**
 * @brief Numerical addition.
 */
ins(ADD)
{
	//DELTA_PRINT_INS_ARGS(ADD);
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_ARG0) + delta_cast_number(DELTA_ARG1));
}


/**
 * @brief Equal to.
 */
ins(CEQ)
{
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_ARG0) == delta_cast_number(DELTA_ARG1));
}


/**
 * @brief Greater than or equal to.
 */
ins(CGE)
{
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_ARG0) >= delta_cast_number(DELTA_ARG1));
}


/**
 * @brief Greater then.
 */
ins(CGT)
{
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_ARG0) > delta_cast_number(DELTA_ARG1));
}


/**
 * @brief Less than or equal to.
 */
ins(CLE)
{
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_ARG0) <= delta_cast_number(DELTA_ARG1));
}


/**
 * @brief Less than.
 */
ins(CLT)
{
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_ARG0) < delta_cast_number(DELTA_ARG1));
}


/**
 * @brief Not equal to.
 */
ins(CNE)
{
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_ARG0) != delta_cast_number(DELTA_ARG1));
}


/**
 * @brief Decrement (subtract 1)
 */
ins(DEC)
{
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_DEST) - 1);
}


/**
 * @brief Numerical division.
 */
ins(DIV)
{
	// this is only numerical division, so other types need to be cast to a number
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_ARG0) / delta_cast_number(DELTA_ARG1));
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
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_DEST) + 1);
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
	DELTA_PRINT_INS_ARGS(MUL);
	
	// this is only numerical multiplicaton, so other types need to be cast to a number
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_ARG0) * delta_cast_number(DELTA_ARG1));
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
	ram[DELTA_DEST]->type = ram[DELTA_ARG0]->type;
	ram[DELTA_DEST]->value.number = ram[DELTA_ARG0]->value.number;
	ram[DELTA_DEST]->value.ptr = ram[DELTA_ARG0]->value.ptr;
}


/**
 * @brief Numerical subtraction.
 */
ins(SUB)
{
	// this is only numerical subtraction, so other types need to be cast to a number
	DELTA_RETURN_NUMBER(delta_cast_number(DELTA_ARG0) - delta_cast_number(DELTA_ARG1));
}

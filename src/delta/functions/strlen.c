/*
 *  Delta programming language
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#include "ins.h"


/**
 * @brief Get the length of a string.
 */
ins(SLN)
{
	delta_cast_string(DELTA_ARG0);
	DELTA_RETURN_NUMBER(ram[DELTA_ARG0]->size);
}

/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <math.h>


/**
 * @category modules/core/math
 * 
 * @brief Returns log(1 + number), computed in a way that is accurate even when the value of number
 *        is close to zero.
 * @syntax number log1p ( number number )
 *
 * log1p() returns log(1 + number) computed in a way that is accurate even when the value of number
 * is close to zero. log() might only return log(1) in this case due to lack of precision.
 * 
 * @param number The argument to process.
 * @return log(1 + number).
 * @see expm1
 * @see log
 * @see log10 
 */
DELTA_FUNCTION(log1p)
{
	DELTA_RETURN_NUMBER(log1p(delta_cast_number(DELTA_ARG0)));
}

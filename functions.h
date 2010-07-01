/*
 *  Delta programming language
 *
 *  Created by Elliot Chance on 01/07/10.
 */

#include "structs.h"


DeltaFunction* new_DeltaFunction(char *name, void (*function_ptr)(struct DeltaInstruction *d),
								 int min_args, int max_args);

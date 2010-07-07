/*
 *  Delta programming language
 */


#ifndef _DELTA_MACROS_H
#define _DELTA_MACROS_H 1


#include "structs.h"


#define DELTA_ADD_BYTECODE(__BYTECODE) \
	printf("BYTECODE_%s (", #__BYTECODE); \
	int _k; \
	arg_ptr[arg_depth][0] = var_dest; \
	for(_k = 0; _k < arg_count[arg_depth]; ++_k) { \
		printf(" %d", arg_ptr[arg_depth][_k]); \
	} \
	printf(" )\n"); \
	DeltaFunction_push(c, new_DeltaInstructionN(BYTECODE_##__BYTECODE));


#define DELTA_ADD_OPERATOR_BYTECODE(__BYTECODE) \
	printf("BYTECODE_%s (%d, %d, %d)\n", #__BYTECODE, var_dest, var_id1, var_id2); \
	DeltaFunction_push(c, new_DeltaInstruction3( \
		NULL, BYTECODE_##__BYTECODE, var_dest, var_id1, var_id2));


#define DELTA_ENUMERATE_ARRAY(__array, __element, __element_id) \
	for(__element_id = 0; \
	    i < __array->elements; \
	    ++__element_id, __element = __element->next)


#define DELTA_INS(name) void ins_##name(struct DeltaInstruction *d)


#define DELTA_FUNCTION(name) void delta_function_##name(struct DeltaInstruction *d)


#define FUNC(name) delta_function_##name


#define DELTA_PRINT_INS_ARGS(__BYTECODE) \
	printf("*BYTECODE_%s (", #__BYTECODE); \
	int i; \
	for(i = 0; i < DELTA_ARGS + 1; ++i) \
	printf(" %d", d->arg[i]); \
	printf(" )\n");


#define DELTA_RETURN_NULL \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_NULL; \
	return;\
}


#define DELTA_RETURN_NAN \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_NUMBER; \
	ram[DELTA_DEST]->value.number = NAN; \
	return;\
}


#define DELTA_RETURN_BOOLEAN(return_value) \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_BOOLEAN; \
	ram[DELTA_DEST]->value.number = (return_value == 0 ? 0 : 1); \
	return; \
}


#define DELTA_RETURN_NUMBER(return_value) \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_NUMBER; \
	ram[DELTA_DEST]->value.number = return_value; \
	return; \
}


#define DELTA_RETURN_ZERO \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_NUMBER; \
	ram[DELTA_DEST]->value.number = 0; \
	return; \
}


#define DELTA_RETURN_STRING(return_value) \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_STRING; \
	ram[DELTA_DEST]->value.ptr = return_value; \
	return; \
}


#define DELTA_RETURN_RESOURCE(return_value, resource_id) \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_RESOURCE; \
	ram[DELTA_DEST]->value.resource.ptr = (char*) return_value; \
	ram[DELTA_DEST]->value.resource.id = resource_id; \
	return; \
}


#define DELTA_ARGS        ((d->args - 1) / 2)
#define DELTA_DEST        d->arg[0]

#define DELTA_ARG0        d->arg[2]
#define DELTA_ARG1        d->arg[4]
#define DELTA_ARG2        d->arg[6]
#define DELTA_ARG3        d->arg[8]
#define DELTA_ARG4        d->arg[10]
#define DELTA_ARG5        d->arg[12]
#define DELTA_ARG6        d->arg[14]
#define DELTA_ARG7        d->arg[16]

#define DELTA_ARG0_NAME   d->arg[1]
#define DELTA_ARG1_NAME   d->arg[3]
#define DELTA_ARG2_NAME   d->arg[5]
#define DELTA_ARG3_NAME   d->arg[7]
#define DELTA_ARG4_NAME   d->arg[9]
#define DELTA_ARG5_NAME   d->arg[11]
#define DELTA_ARG6_NAME   d->arg[13]
#define DELTA_ARG7_NAME   d->arg[15]

#define DELTA_ARG(i)      d->arg[(i * 2) + 2]
#define DELTA_ARG_NAME(i) d->arg[(i * 2) + 1]


#define DELTA_SUCCESS 1
#define DELTA_FAILURE 0
#define DELTA_YES     1
#define DELTA_NO      0
#define DELTA_TRUE    1
#define DELTA_FALSE   0


#define DELTA_TYPE_NULL     0
#define DELTA_TYPE_BOOLEAN  1
#define DELTA_TYPE_NUMBER   2
#define DELTA_TYPE_STRING   3
#define DELTA_TYPE_ARRAY    4
#define DELTA_TYPE_RESOURCE 5
#define DELTA_TYPE_OBJECT   6


#endif

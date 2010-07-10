/*
 *  Delta programming language
 */


#ifndef _DELTA_MACROS_H
#define _DELTA_MACROS_H 1


#include "structs.h"
#include <string.h>


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
	DELTA_DEST->type = DELTA_TYPE_NULL; \
	return;\
}


#define DELTA_RETURN_NAN \
{ \
	DELTA_DEST->type = DELTA_TYPE_NUMBER; \
	DELTA_DEST->value.number = NAN; \
	return;\
}


#define DELTA_RETURN_BOOLEAN(return_value) \
{ \
	DELTA_DEST->type = DELTA_TYPE_BOOLEAN; \
	DELTA_DEST->value.number = (return_value == 0 ? 0 : 1); \
	return; \
}


#define DELTA_RETURN_NUMBER(return_value) \
{ \
	DELTA_DEST->type = DELTA_TYPE_NUMBER; \
	DELTA_DEST->value.number = return_value; \
	return; \
}


#define DELTA_RETURN_ZERO \
{ \
	DELTA_DEST->type = DELTA_TYPE_NUMBER; \
	DELTA_DEST->value.number = 0; \
	return; \
}


#define DELTA_RETURN_ONE \
{ \
	DELTA_DEST->type = DELTA_TYPE_NUMBER; \
	DELTA_DEST->value.number = 1; \
	return; \
}


#define DELTA_RETURN_FALSE \
{ \
	DELTA_DEST->type = DELTA_TYPE_BOOLEAN; \
	DELTA_DEST->value.number = DELTA_FALSE; \
	return; \
}


#define DELTA_RETURN_TRUE \
{ \
	DELTA_DEST->type = DELTA_TYPE_BOOLEAN; \
	DELTA_DEST->value.number = DELTA_TRUE; \
	return; \
}


#define DELTA_RETURN_STRING(return_value) \
{ \
	DELTA_DEST->type = DELTA_TYPE_STRING; \
	DELTA_DEST->value.ptr = return_value; \
	DELTA_DEST->size = strlen(return_value); \
	return; \
}


#define DELTA_RETURN_BINARY_STRING(return_value, return_size) \
{ \
	DELTA_DEST->type = DELTA_TYPE_STRING; \
	DELTA_DEST->value.ptr = return_value; \
	DELTA_DEST->size = return_size; \
	return; \
}


#define DELTA_RETURN_RESOURCE(return_value, resource_id) \
{ \
	DELTA_DEST->type = DELTA_TYPE_RESOURCE; \
	DELTA_DEST->value.resource.ptr = (char*) return_value; \
	DELTA_DEST->value.resource.id = resource_id; \
	return; \
}


/**
 * @brief Returns the number of arguments sent to the function.
 */
#define DELTA_ARGS ((d->args - 1) / 2)


/**
 * @brief The RAM address of the destination variable.
 */
#define DELTA_DEST d->varg[0]


// Argument addresses
#define DELTA_ARG0 d->varg[2]
#define DELTA_ARG1 d->varg[4]
#define DELTA_ARG2 d->varg[6]
#define DELTA_ARG3 d->varg[8]
#define DELTA_ARG4 d->varg[10]
#define DELTA_ARG5 d->varg[12]
#define DELTA_ARG6 d->varg[14]
#define DELTA_ARG7 d->varg[16]


// Argument name addresses, these are used like char*
#define DELTA_ARG0_NAME d->varg[1]
#define DELTA_ARG1_NAME d->varg[3]
#define DELTA_ARG2_NAME d->varg[5]
#define DELTA_ARG3_NAME d->varg[7]
#define DELTA_ARG4_NAME d->varg[9]
#define DELTA_ARG5_NAME d->varg[11]
#define DELTA_ARG6_NAME d->varg[13]
#define DELTA_ARG7_NAME d->varg[15]


/**
 * @brief Get argument value by argument ID.
 */
#define DELTA_ARG(i) d->varg[(i * 2) + 2]


/**
 * @brief Get argument name by argument ID.
 */
#define DELTA_ARG_NAME(i) d->varg[(i * 2) + 1]


#define DELTA_SUCCESS 1
#define DELTA_FAILURE 0
#define DELTA_YES     1
#define DELTA_NO      0
#define DELTA_TRUE    1
#define DELTA_FALSE   0

/**
 * @brief Return a NULL value.
 */
#define DELTA_TYPE_NULL 0


/**
 * @brief Return a boolean value.
 * This macro expects a number, if the value is 0 then FALSE will be returned, any other number will
 * be set to 1 and hence TRUE.
 */
#define DELTA_TYPE_BOOLEAN 1


/**
 * @brief Return a number value.
 */
#define DELTA_TYPE_NUMBER 2


/**
 * @brief Return a string value.
 */
#define DELTA_TYPE_STRING 3


/**
 * @brief Return an array value.
 */
#define DELTA_TYPE_ARRAY 4


/**
 * @brief Return a resource.
 */
#define DELTA_TYPE_RESOURCE 5


/**
 * @brief Return an object.
 */
#define DELTA_TYPE_OBJECT 6


/**
 * @brief For finding variable safe types.
 */
#define DELTA_TYPE_UNKNOWN 10


/**
 * @brief Check if a variable is a valid resource of a given type.
 */
#define DELTA_CHECK_RESOURCE(__resource, __type) \
	(__resource->type == DELTA_TYPE_RESOURCE && __resource->value.resource.id == __type)


#define DELTA_ERROR_ERROR "1"

#define DELTA_ERROR_WARNING "2"

#define DELTA_ERROR_PARSE "4"

#define DELTA_ERROR_NOTICE "8"

#define DELTA_ERROR_USER_ERROR "256"

#define DELTA_ERROR_USER_WARNING "512"

#define DELTA_ERROR_USER_NOTICE "1024"

/**
 * @brief Trigger a runtime error.
 * @param msg A char* message
 * @param type Can be one of DELTA_ERROR_*
 */
#define DELTA_TRIGGER_ERROR(msg, type) \
	delta_vm_push_runtime_error(msg, atoi(type))


#define DELTA_RELEASE(obj_release, obj_ptr) \
{ \
	if(obj_release) \
		delta_release_variable(obj_ptr); \
}


#endif

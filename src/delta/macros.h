/*
 *  Delta programming language
 */


#ifndef _DELTA_MACROS_H
#define _DELTA_MACROS_H 1


#include "platform.h"
#include "structs.h"
#include <string.h>


#define DELTA_ADD_BYTECODE(__BYTECODE) \
{ \
	struct DeltaInstruction _dI = new_DeltaInstructionN(__BYTECODE);\
	delta_bytecode_writer_line(c, __BYTECODE, #__BYTECODE, line_number, "", _dI, \
		c->total_functions); \
	arg_ptr[arg_depth][0] = var_dest; \
	DeltaFunction_push(c, c->total_functions, _dI); \
}


#define DELTA_ADD_OPERATOR_BYTECODE(__BYTECODE) \
{ \
	struct DeltaInstruction _dI = \
		new_DeltaInstruction3(NULL, __BYTECODE, var_dest, var_id1, var_id2); \
	delta_bytecode_writer_line(c, __BYTECODE, #__BYTECODE, line_number, "", _dI, \
		c->total_functions); \
	DeltaFunction_push(c, c->total_functions, _dI); \
}


#define DELTA_WRITE_BYTECODE(__bytecode, __msg, __id, __ins) \
{ \
	delta_bytecode_writer_line(c, __bytecode, #__bytecode, line_number, __msg, __ins, __id); \
	DeltaFunction_push(c, __id, __ins); \
}


#define DELTA_WRITE_UNLINED_BYTECODE(__bytecode, __msg, __id, __ins) \
{ \
	delta_bytecode_writer_line(c, __bytecode, #__bytecode, 0, __msg, __ins, __id); \
	DeltaFunction_push(c, __id, __ins); \
}


/**
 * @brief Fast enumerate the elements of an array.
 *
 * Example (based on array_sum() function):
 * @code
 * double total = 0;
 * int i;
 * struct DeltaArrayValue *element;
 * 
 * DELTA_ENUMERATE_ARRAY(&DELTA_ARG0->value.array, element, i)
 *   total += delta_cast_number(element->value);
 * @endcode
 */
#define DELTA_ENUMERATE_ARRAY(__array, __element, __element_id) \
	for(__element_id = 0, __element = (__array)->head; \
	    i < (__array)->elements; \
	    ++__element_id, __element = (__element)->next)


#define DELTA_INS(name) void ins_##name(struct DeltaInstruction *d)


#define DELTA_FUNCTION(name) void delta_function_##name(struct DeltaInstruction *d)


#define FUNC(name) delta_function_##name


#define DELTA_PRINT_ARGS() \
{ \
	int i; \
	for(i = 0; i < d->args; ++i) \
		printf(" %d", d->arg[i]); \
	printf(" )\n"); \
}


/**
 * @brief Return NULL.
 */
#define DELTA_RETURN_NULL \
{ \
	DELTA_DEST->type = DELTA_TYPE_NULL; \
	return;\
}


/**
 * @brief Return NaN constant.
 *
 * NaN (Not A Number) is a special constant to signify that an operation was made that could not be
 * represented. An example is a negative sqrt() or divide by zero.
 */
#define DELTA_RETURN_NAN \
{ \
	DELTA_DEST->type = DELTA_TYPE_NUMBER; \
	DELTA_DEST->value.number = NAN; \
	return;\
}


/**
 * @brief Return BOOLEAN value.
 *
 * @param return_value Must be an numerical type (int, double etc) where 0 is FALSE and any other
 *        number (including nagatives) is considered TRUE.
 */
#define DELTA_RETURN_BOOLEAN(return_value) \
{ \
	DELTA_DEST->type = DELTA_TYPE_BOOLEAN; \
	DELTA_DEST->value.number = (return_value == 0 ? 0 : 1); \
	return; \
}


/**
 * @brief Return a NUMBER.
 */
#define DELTA_RETURN_NUMBER(return_value) \
{ \
	DELTA_DEST->type = DELTA_TYPE_NUMBER; \
	DELTA_DEST->value.number = return_value; \
	return; \
}


/**
 * @brief Return 0.
 *
 * This is just a more convenient way of using:
 * @code
 * DELTA_RETURN_NUMBER(0);
 * @endcode
 */
#define DELTA_RETURN_ZERO \
{ \
	DELTA_DEST->type = DELTA_TYPE_NUMBER; \
	DELTA_DEST->value.number = 0; \
	return; \
}


/**
 * @brief Return 1.
 *
 * This is just a more convenient way of using:
 * @code
 * DELTA_RETURN_NUMBER(1);
 * @endcode
 */
#define DELTA_RETURN_ONE \
{ \
	DELTA_DEST->type = DELTA_TYPE_NUMBER; \
	DELTA_DEST->value.number = 1; \
	return; \
}


/**
 * @brief Return BOOLEAN FALSE.
 */
#define DELTA_RETURN_FALSE \
{ \
	DELTA_DEST->type = DELTA_TYPE_BOOLEAN; \
	DELTA_DEST->value.number = DELTA_FALSE; \
	return; \
}


/**
 * @brief Return BOOLEAN TRUE.
 */
#define DELTA_RETURN_TRUE \
{ \
	DELTA_DEST->type = DELTA_TYPE_BOOLEAN; \
	DELTA_DEST->value.number = DELTA_TRUE; \
	return; \
}


/**
 * @brief Return null-terminated STRING.
 *
 * If there is a chance your string will constain binary information use DELTA_RETURN_BINARY_STRING.
 */
#define DELTA_RETURN_STRING(return_value) \
{ \
	DELTA_DEST->type = DELTA_TYPE_STRING; \
	DELTA_DEST->value.ptr = return_value; \
	DELTA_DEST->size = strlen(return_value); \
	return; \
}


/**
 * @brief Return single character as a STRING.
 */
#define DELTA_RETURN_CHAR(return_value) \
{ \
	DELTA_DEST->type = DELTA_TYPE_STRING; \
	DELTA_DEST->value.ptr = (char*) malloc(2); \
	DELTA_DEST->value.ptr[0] = (char) (return_value); \
	DELTA_DEST->value.ptr[1] = '\0'; \
	DELTA_DEST->size = 1; \
	return; \
}


/**
 * @brief Return binary-safe STRING.
 *
 * You only really need to use this if there is a chance your string will constain binary
 * characters, otherwise you can use DELTA_RETURN_STRING.
 */
#define DELTA_RETURN_BINARY_STRING(return_value, return_size) \
{ \
	DELTA_DEST->type = DELTA_TYPE_STRING; \
	DELTA_DEST->value.ptr = return_value; \
	DELTA_DEST->size = return_size; \
	return; \
}


/**
 * @brief Return RESOURCE handle.
 *
 * @param return_value Must be a pointer of any type.
 * @param resource_id Must be an integer that uniquely identifies this resource type.
 */
#define DELTA_RETURN_RESOURCE(return_value, resource_id) \
{ \
	DELTA_DEST->type = DELTA_TYPE_RESOURCE; \
	DELTA_DEST->value.resource.ptr = (char*) return_value; \
	DELTA_DEST->value.resource.id = resource_id; \
	return; \
}


/**
 * @brief Return ARRAY.
 *
 * @param __array Must be a non-pointer object of DeltaArray.
 */
#define DELTA_RETURN_ARRAY(__array) \
{ \
	DELTA_DEST->type = DELTA_TYPE_ARRAY; \
	DELTA_DEST->value.array = __array; \
	return; \
}


/**
 * @brief Return a variable of any type.
 *
 * This will NOT copy the returned variable, so do not free it.
 */
#define DELTA_RETURN_VARIABLE(__var) \
{ \
	DELTA_COPY_VARIABLE(DELTA_DEST, __var); \
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
#define DELTA_ARG0_NAME delta_cast_new_string(d->varg[1], NULL)
#define DELTA_ARG1_NAME delta_cast_new_string(d->varg[3], NULL)
#define DELTA_ARG2_NAME delta_cast_new_string(d->varg[5], NULL)
#define DELTA_ARG3_NAME delta_cast_new_string(d->varg[7], NULL)
#define DELTA_ARG4_NAME delta_cast_new_string(d->varg[9], NULL)
#define DELTA_ARG5_NAME delta_cast_new_string(d->varg[11], NULL)
#define DELTA_ARG6_NAME delta_cast_new_string(d->varg[13], NULL)
#define DELTA_ARG7_NAME delta_cast_new_string(d->varg[15], NULL)


/**
 * @brief Get argument value by argument ID.
 */
#define DELTA_ARG(i) d->varg[(i * 2) + 2]


/**
 * @brief Get argument name by argument ID.
 *
 * This will always be returned as a char*
 */
#define DELTA_ARG_NAME(i) delta_cast_new_string(d->varg[(i * 2) + 1], NULL)


#define DELTA_SUCCESS 1
#define DELTA_FAILURE 0
#define DELTA_YES     1
#define DELTA_NO      0


/**
 * @brief Constant for TRUE.
 *
 * It is recommended you use this constant when you are returning the delta boolean value of TRUE.
 */
#define DELTA_TRUE 1


/**
 * @brief Constant for FALSE.
 *
 * It is recommended you use this constant when you are returning the delta boolean value of FALSE.
 */
#define DELTA_FALSE 0


#define DELTA_MAX_FUNCTIONS 16


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


#define DELTA_PREPARE_MODULE_FUNCTIONS(_total) \
	int stack_i = 0; \
	*count = _total; \
	struct DeltaModuleFunction *f = (struct DeltaModuleFunction*) \
		calloc(_total, sizeof(struct DeltaModuleFunction));


#define DELTA_PUSH_FUNCTION(_name, _min, _max) \
	f[stack_i].name = #_name; \
	f[stack_i].min_args = _min; \
	f[stack_i].max_args = _max; \
	f[stack_i].is_static = DELTA_YES; \
	f[stack_i].permission = DELTA_PUBLIC; \
	++stack_i;


#define DELTA_END_MODULE_FUNCTIONS \
	*count = stack_i; \
	return f


#define DELTA_PREPARE_MODULE_DEFINES(_total) \
	int stack_i = 0; \
	*count = _total; \
	struct DeltaDefine *f = (struct DeltaDefine*) \
		calloc(_total, sizeof(struct DeltaDefine));


#define DELTA_PUSH_DEFINE(_name, _value) \
	f[stack_i].name = #_name; \
	f[stack_i].value = _value; \
	++stack_i;


#define DELTA_END_MODULE_DEFINES \
	*count = stack_i; \
	return f


#define DELTA_COPY_VARIABLE(__dest, __src) \
{ \
	(__dest)->type = (__src)->type; \
	(__dest)->size = (__src)->size; \
	(__dest)->value.number = (__src)->value.number; \
	(__dest)->value.ptr = (__src)->value.ptr; \
	(__dest)->value.array = (__src)->value.array; \
	(__dest)->value.resource = (__src)->value.resource; \
}


#define DELTA_PREPARE_MODULE_FUNCTION_ALIASES(_total) \
	int stack_i = 0; \
	*count = _total; \
	struct DeltaFunctionAlias *f = (struct DeltaFunctionAlias*) \
		calloc(_total, sizeof(struct DeltaFunctionAlias));


#define DELTA_PUSH_FUNCTION_ALIAS(_alias, _min, _max, _real) \
	f[stack_i].alias = #_alias; \
	f[stack_i].original = #_real; \
	f[stack_i].min_args = _min; \
	f[stack_i].max_args = _max; \
	f[stack_i].is_static = DELTA_YES; \
	f[stack_i].permission = DELTA_PUBLIC; \
	++stack_i;


#define DELTA_END_MODULE_FUNCTION_ALIASES \
	*count = stack_i; \
	return f


#define DELTA_MAIN_FUNCTION "main"


#define DELTA_MAX_ARGS 1024


#define DELTA_CONCAT_OPERATOR "@"


#define DELTA_OBJECT_OPERATOR "."


#define DELTA_PRIVATE 0


#define DELTA_PROTECTED 1


#define DELTA_PUBLIC 2


#endif

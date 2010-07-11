/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "delta/compiler/compiler.h"
#include "delta/compiler/strings.h"
#include <sys/time.h>


/**
 * @brief Generates a user-level error/warning/notice message.
 * @syntax bool trigger_error ( string error_msg [, int error_type = E_USER_NOTICE ] )
 *
 * Used to trigger a user error condition, it can be used by in conjunction with the built-in error
 * handler, or with a user defined function that has been set as the new error handler
 * (set_error_handler()). This function is useful when you need to generate a particular response to
 * an exception at runtime.
 *
 * @param error_msg The designated error message for this error. It's limited to 1024 characters in
 *        length. Any additional characters beyond 1024 will be truncated.
 * @param error_type The designated error type for this error. It only works with the E_USER family
 *        of constants, and will default to E_USER_NOTICE.
 * @return This function returns FALSE if wrong error_type is specified, TRUE otherwise.
 */
DELTA_FUNCTION(trigger_error)
{
	int release0;
	struct DeltaVariable *error_msg = delta_cast_string(DELTA_ARG0, &release0);
	int error_type = atoi(DELTA_ERROR_USER_NOTICE);
	
	if(DELTA_ARGS > 1)
		error_type = delta_cast_number(DELTA_ARG1);
	
	int r;
	if(release0)
		r = delta_vm_push_runtime_error(error_msg->value.ptr, error_type);
	else
		r = delta_vm_push_runtime_error(delta_copy_string(error_msg->value.ptr), error_type);
	
	DELTA_RETURN_BOOLEAN(r);
}

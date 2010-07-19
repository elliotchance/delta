/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "include/mysql.h"


DELTA_FUNCTION(mysql_query)
{
	// check for valid resource
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_MYSQL_CONN)) {
		DELTA_TRIGGER_ERROR("Invalid resource type in mysql_query()", DELTA_ERROR_WARNING);
		DELTA_RETURN_NULL;
	}
	
	char *sql = delta_cast_new_string(DELTA_ARG1, NULL);
	MYSQL *conn = (MYSQL*) DELTA_ARG0->value.resource.ptr;
	
	mysql_query(conn, sql);
	MYSQL_RES *result = mysql_store_result(conn);
	
	free(sql);
	DELTA_RETURN_RESOURCE(result, DELTA_RESOURCE_MYSQL_QUERY);
}

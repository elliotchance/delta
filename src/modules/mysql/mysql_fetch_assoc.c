/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "include/mysql.h"


DELTA_FUNCTION(mysql_fetch_assoc)
{
	// check for valid resource
	if(!DELTA_CHECK_RESOURCE(DELTA_ARG0, DELTA_RESOURCE_MYSQL_QUERY)) {
		DELTA_TRIGGER_ERROR("Invalid resource type in mysql_fetch_assoc()", DELTA_ERROR_WARNING);
		DELTA_RETURN_NULL;
	}
	
	MYSQL_RES *result = (MYSQL_RES*) DELTA_ARG0->value.resource.ptr;
	if(result == NULL) {
		DELTA_TRIGGER_ERROR("NULL query in mysql_fetch_assoc()", DELTA_ERROR_WARNING);
		DELTA_RETURN_NULL;
	}
	
	// convert the new row into an associative array
	int columns = mysql_num_fields(result), i;
	MYSQL_ROW row;
	if(!(row = mysql_fetch_row(result)))
	   DELTA_RETURN_FALSE;
	struct DeltaArray r = delta_new_array();
	MYSQL_FIELD *fields = mysql_fetch_fields(result);
	
	for(i = 0; i < columns; ++i)
		delta_array_push_k_string(&r, fields[i].name, row[i]);
	
	DELTA_RETURN_ARRAY(r);
}

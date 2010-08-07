/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "module.h"
#include "include/mysql.h"


/**
 * @category modules/mysql
 *
 * @brief Connect to a MySQL server.
 */
DELTA_FUNCTION(mysql_connect)
{
	// get arguments
	char *host = NULL, *user = NULL, *pass = NULL, *db = NULL;
	
	int i;
	for(i = 0; i < DELTA_ARGS; ++i) {
		if(!stricmp(DELTA_ARG_NAME(i), "host"))
			host = delta_cast_new_string(DELTA_ARG(i), NULL);
		if(!stricmp(DELTA_ARG_NAME(i), "user"))
			user = delta_cast_new_string(DELTA_ARG(i), NULL);
		if(!stricmp(DELTA_ARG_NAME(i), "pass"))
			pass = delta_cast_new_string(DELTA_ARG(i), NULL);
		if(!stricmp(DELTA_ARG_NAME(i), "db"))
			db = delta_cast_new_string(DELTA_ARG(i), NULL);
	}
	
	// fill in defaults
	if(host == NULL)
		host = "localhost";
	
	// make connection
	MYSQL *mysql = mysql_init(NULL);
	mysql_real_connect(mysql, host, user, pass, db, 0, NULL, 0);
	
	DELTA_RETURN_RESOURCE(mysql, DELTA_RESOURCE_MYSQL_CONN);
}

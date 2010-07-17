/*
 *  Delta programming language
 */

#include "delta/delta.h"


#define DELTA_RESOURCE_SQLITE3_CONN  0x9874
#define DELTA_RESOURCE_SQLITE3_QUERY 0x9875


struct DeltaModuleFunction* module_functions(int *count);

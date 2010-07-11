/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include "mapm-external/m_apm.h"


extern int mapm_precision;


struct DeltaModuleFunction* module_functions(int *count);
M_APM                       apm_get(struct DeltaVariable *v);

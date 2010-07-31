/*
 *  Delta programming language
 */


#include "delta/delta.h"
#include <pthread.h>


static pthread_t threads[32];
static int total_threads = 0;
extern struct DeltaVM *DELTA_VM;


struct DeltaModuleFunction* module_functions(int *count);
void                        module_set_vm(struct DeltaVM *vm);
void                        thread_detach(stack_function func);

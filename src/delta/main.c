/*
 *  Delta programming language
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <math.h>

#include "delta/structs.h"
#include "delta/load/save.h"
#include "delta/load/load.h"
#include "delta/vm/vm.h"
#include "delta/compiler/compiler.h"
#include "delta/jit/jit_compiler.h"
#include "delta/vm/modules.h"


#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000000
#endif


void main_compile()
{
	struct DeltaCompiler *c = delta_compiler_init();
	delta_compile_file(c, "test.delta");
	delta_save_file(c, "test.dc");
}


void main_run()
{
	struct DeltaVM *vm = delta_load_file("test.dc");
	delta_set_vm(vm);
	delta_load_modules(vm);
	stack_function delta_entry = delta_compile_jit(vm, DELTA_MAIN_FUNCTION);
	
	long start = clock() - 1;
	printf("\n\n==> BEGIN, hit enter to proceed\n");
	delta_entry(NULL);
	printf("==> END\n\n");
	printf("time: %.3f\n", (double) (clock() - start) / (double) CLOCKS_PER_SEC);
	
	delta_vm_print_ram(vm);
}


int main()
{
	main_compile();
	main_run();
}

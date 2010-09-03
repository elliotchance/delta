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
	printf(">>>> Compiling\n");
	struct DeltaCompiler *c = delta_compiler_init();
	delta_load_defines(c);
	delta_compile_file(c, "test.delta");
	delta_save_file(c, "test.dc", "test.delta");
	printf(">>>> Compiling Done\n");
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
	delta_load_ini();
	// FIXME: turn delta_needs_compile() on when ready
	//if(delta_needs_compile("test.delta", "test.dc"))
		main_compile();
	main_run();
}

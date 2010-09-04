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
#include "delta/jit/virtual_compiler.h"
#include "delta/vm/modules.h"
#include "delta/structs/DeltaCompiler.h"
#include "delta/structs/DeltaVM.h"
#include "delta/structs/DeltaFunction.h"


#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000000
#endif


#define VIRTUAL_COMPILE 1


void main_compile()
{
	struct DeltaCompiler *c = delta_compiler_init();
	delta_load_defines(c);
	delta_compile_file(c, "test.delta");
	delta_save_file(c, "test.dc", "test.delta");
	
	// stop on errors
	if(c->errors)
		delta_die("Stopping compilation due to errors.\n");
	
	free_DeltaCompiler(c);
}


void main_run()
{
	struct DeltaVM *vm = delta_load_file("test.dc");
	delta_set_vm(vm);
	delta_load_modules(vm);
#ifndef VIRTUAL_COMPILE
	delta_jit_function delta_entry = delta_compile_jit(vm, DELTA_MAIN_FUNCTION);
#else
	struct delta_virtual_function *delta_entry = delta_compile_virtual(vm, DELTA_MAIN_FUNCTION);
#endif
	
	long start = clock() - 1;
	printf("\n\n==> BEGIN\n");
#ifdef VIRTUAL_COMPILE
	delta_run_virtual(delta_entry);
#else
	delta_entry(NULL);
#endif
	printf("==> END\n\n");
	printf("time: %.3f\n", (double) (clock() - start) / (double) CLOCKS_PER_SEC);
	
	delta_vm_print_ram(vm);
	free_DeltaVM(vm);
}


int main()
{
	delta_load_ini();
	// FIXME: turn delta_needs_compile() on when ready
	//if(delta_needs_compile("test.delta", "test.dc"))
		main_compile();
	main_run();
}

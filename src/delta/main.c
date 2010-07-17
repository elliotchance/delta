/*
 *  Delta programming language
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <math.h>
#include "delta/jit/lightning.h"

#include "delta/structs.h"
#include "delta/compiler/bytecode.h"
#include "delta/compiler/compiler.h"
#include "delta/load/save.h"
#include "delta/vm/vm.h"
#include "delta/jit/jit_compiler.h"
#include "delta/jit/optimiser.h"
#include "delta/structs/DeltaCompiler.h"


int main()
{
	// init the compiler
	struct DeltaCompiler *c = new_DeltaCompiler();
	delta_vm_init(c);
	
	// take input files
	delta_compile_file(c, "test.delta");
	
	// optimise bytecode
	//delta_optimise_bytecode(c, 0, c->total_ins);
	
	// save bytecode
	delta_save_file(c, "test.dc");
	
	// compile bytecode to jit
	stack_function f = delta_compile_jit(c, 0, c->total_ins);
	
	// execute
	delta_vm_prepare(c);
	start = clock();
	printf("\n\n==> BEGIN, hit enter to proceed\n");
	//getchar();
	f(NULL);
	printf("==> END\n\n");
	printf("time: %.3f\n", (double) (clock() - start) / (double) CLOCKS_PER_SEC);
	
	// show ram
	delta_vm_print_ram(c);
}

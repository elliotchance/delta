/*
 *  Delta programming language
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <math.h>
#include "lightning.h"

#include "structs.h"
#include "bytecode.h"
#include "compiler.h"
#include "save.h"
#include "vm.h"
#include "jit_compiler.h"


int main()
{
	// init the compiler
	DeltaCompiler *c = new_DeltaCompiler();
	
	// take input files
	delta_compile_file(c, "test.delta");
	
	// save bytecode
	delta_save_file(c, "test.dc");
	
	// compile bytecode to jit
	delta_vm_init(c);
	stack_function f = delta_compile_jit(c, 0, c->total_ins);
	
	// execute
	delta_vm_prepare(c);
	start = clock();
	printf("\n\n==> BEGIN\n");
	f(NULL);
	printf("==> END\n\n");
	printf("time: %.3f\n", (double) (clock() - start) / (double) CLOCKS_PER_SEC);
	
	// show ram
	delta_vm_print_ram(c);
}

/*
 *  Delta programming language
 */

#include "virtual_compiler.h"
#include "delta/compiler/bytecode.h"
#include "delta/vm/vm.h"
#include "delta/jit/ins.h"
#include "delta/jit/jit_compiler.h"
#include "delta/compiler/strings.h"
#include <assert.h>
#include <string.h>
#include <ctype.h>


#define ADD_VIRTUAL_BYTECODE(_func) \
{ \
	r->ins[r->total_ins].func = _func; \
	r->ins[r->total_ins].ins = instructions[i]; \
	++r->total_ins; \
}


struct delta_virtual_function* delta_compile_virtual(struct DeltaVM *vm, char *function_name)
{
	int i, j, loop_id = 0, end = 0, function_id = -1;
	struct DeltaInstruction *instructions = NULL;
	int total_ram = 100, total_static_ram = 10;
	
	// try to find the function
	for(i = 0; i < vm->total_functions; ++i) {
		if(!strcmp(function_name, vm->functions[i].name)) {
			// found it, if the function is already JIT compiled then great we saved some time
			if(vm->functions[i].virtual_ptr != NULL)
				return vm->functions[i].virtual_ptr;
			
			instructions = vm->functions[i].ins;
			end = vm->functions[i].total_ins;
			function_id = i;
			break;
		}
	}
	assert(function_id >= 0);
	
	// because functions cannot be recursivly compiled, JIT_V0 etc would be overriden we first
	// take a quick look through the instructions and compile any required functions first
	for(i = 0; i < end; ++i) {
		if(instructions[i].bc == BYTECODE_CAL) {
			// check if it is a user function
			for(j = 0; j < vm->total_functions; ++j) {
				if(!strcmp(vm->functions[j].name, instructions[i].func)) {
					// FIXME: dont compile yourself
					vm->functions[j].virtual_ptr = delta_compile_virtual(vm, instructions[i].func);
					break;
				}
			}
		}
	}
	
	// create a new RAM
	struct DeltaVariable **ram = (struct DeltaVariable**)
	calloc(total_ram, sizeof(struct DeltaVariable*));
	for(i = 0; i < total_ram; ++i)
		ram[i] = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable));
	
	// static RAM for interfunction communications
	static struct DeltaVariable **static_ram = NULL;
	if(static_ram == NULL) {
		static_ram = (struct DeltaVariable**)
		calloc(total_static_ram, sizeof(struct DeltaVariable*));
		for(i = 0; i < total_static_ram; ++i)
			static_ram[i] = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable));
	}
	
	// TODO: this will have to be changed for multithreaded apps, for now we'll just add the ram to
	// the function
	vm->functions[function_id].ram = ram;
	
	// prepare
	struct delta_virtual_function *r =
		(struct delta_virtual_function*) malloc(sizeof(struct delta_virtual_function));
	r->alloc_ins = 100;
	r->total_ins = 0;
	r->ins = (delta_virtual_instruction*) calloc(r->alloc_ins, sizeof(delta_virtual_instruction));
	
	for(i = 0; i < end; ++i) {
#if 0
		// show bytecode
		printf("VC {%d} %d:", function_id, instructions[i].bc);
		for(j = 0; j < instructions[i].args; ++j)
			printf(" %d", instructions[i].arg[j]);
		printf("\n");
#endif
		
		// skip NUL bytecodes
		if(instructions[i].bc == BYTECODE_NUL)
			continue;
		
		if(instructions[i].bc == BYTECODE_IFS) {
			assert(!"BYTECODE_IFS not supported with virtual compiler");
		}
		else if(instructions[i].bc == BYTECODE_LOP) {
			assert(!"BYTECODE_LOP not supported with virtual compiler");
		}
		else if(instructions[i].bc == BYTECODE_LBL) {
			assert(!"BYTECODE_LBL not supported with virtual compiler");
		}
		else if(instructions[i].bc == BYTECODE_GTO) {
			assert(!"BYTECODE_GTO not supported with virtual compiler");
		}
		else if(instructions[i].bc == BYTECODE_PAT) {
			assert(!"BYTECODE_PAT not supported with virtual compiler");
		}
		else if(instructions[i].bc == BYTECODE_JMP) {
			assert(!"BYTECODE_JMP not supported with virtual compiler");
		}
		else {
			// link argument addresses to virtual RAM locations
			instructions[i].varg = (struct DeltaVariable**)
				calloc(instructions[i].args, sizeof(struct DeltaVariable*));
			
			for(j = 0; j < instructions[i].args; ++j) {
				if(instructions[i].arg[j] < 0)
					instructions[i].varg[j] = static_ram[-instructions[i].arg[j]];
				else
					instructions[i].varg[j] = ram[instructions[i].arg[j]];
			}
			
			// link the function by its name
			if(instructions[i].bc == BYTECODE_CAL) {
				delta_jit_function linked = NULL;
				int fargs = (instructions[i].args - 1) / 2;
				
				// built-in functions
				for(j = 0; j < vm->total_delta_functions; ++j) {
					if(!stricmp(vm->delta_functions[j]->name, instructions[i].func) &&
					   fargs >= vm->delta_functions[j]->min_args &&
					   fargs <= vm->delta_functions[j]->max_args) {
						delta_check_static(vm, i, j, instructions);
						linked = vm->delta_functions[j]->function_ptr;
						break;
					}
				}
				
				// before the actual compiled function can be called we need to call a joining
				// function that takes care of things like stack backtrace and preparing function
				// arguments
				if(linked == NULL) {
					//assert(!"User functions not supported with virtual compiler");
					//jit_finish(ins_CAL);
				}
				
				// if the function could not be found, then maybe its a user function
				if(linked == NULL) {
					for(j = 0; j < vm->total_functions; ++j) {
						if(!stricmp(vm->functions[j].name, instructions[i].func)) {
							delta_check_permission(vm, i, j, instructions);
							delta_check_static(vm, i, j, instructions);
							linked = ins_UFN;
							instructions[i].virtual_ptr = vm->functions[j].virtual_ptr;
							break;
						}
					}
				}
				
				if(linked == NULL) {
					printf("Delta VM (virtual) Runtime Error: Cannot link function '%s' (bytecode = 0x%x)\n",
						   instructions[i].func, instructions[i].bc);
					//printf("Candidates (%d) are:\n", c->total_delta_functions);
					//for(j = 0; j < c->total_delta_functions; ++i)
					//	printf("  %s\n", c->delta_functions[i]->name);
					exit(1);
				}
				
				ADD_VIRTUAL_BYTECODE(linked);
			}
			else {
				// mostly operator instructions
				     if(0) { }
				else if(instructions[i].bc == BYTECODE_ADD) ADD_VIRTUAL_BYTECODE(ins_ADD)
				else if(instructions[i].bc == BYTECODE_AG1) ADD_VIRTUAL_BYTECODE(ins_AG1)
				else if(instructions[i].bc == BYTECODE_ARG) ADD_VIRTUAL_BYTECODE(ins_ARG)
				else if(instructions[i].bc == BYTECODE_AS1) ADD_VIRTUAL_BYTECODE(ins_AS1)
				else if(instructions[i].bc == BYTECODE_CEQ) ADD_VIRTUAL_BYTECODE(ins_CEQ)
				else if(instructions[i].bc == BYTECODE_CGE) ADD_VIRTUAL_BYTECODE(ins_CGE)
				else if(instructions[i].bc == BYTECODE_CGT) ADD_VIRTUAL_BYTECODE(ins_CGT)
				else if(instructions[i].bc == BYTECODE_CLE) ADD_VIRTUAL_BYTECODE(ins_CLE)
				else if(instructions[i].bc == BYTECODE_CLT) ADD_VIRTUAL_BYTECODE(ins_CLT)
				else if(instructions[i].bc == BYTECODE_CNE) ADD_VIRTUAL_BYTECODE(ins_CNE)
				else if(instructions[i].bc == BYTECODE_DEC) ADD_VIRTUAL_BYTECODE(ins_DEC)
				else if(instructions[i].bc == BYTECODE_DIV) ADD_VIRTUAL_BYTECODE(ins_DIV)
				else if(instructions[i].bc == BYTECODE_INC) ADD_VIRTUAL_BYTECODE(ins_INC)
				else if(instructions[i].bc == BYTECODE_MOD) ADD_VIRTUAL_BYTECODE(ins_MOD)
				else if(instructions[i].bc == BYTECODE_MUL) ADD_VIRTUAL_BYTECODE(ins_MUL)
				else if(instructions[i].bc == BYTECODE_SET) ADD_VIRTUAL_BYTECODE(ins_SET)
				else if(instructions[i].bc == BYTECODE_SUB) ADD_VIRTUAL_BYTECODE(ins_SUB)
				else if(instructions[i].bc == BYTECODE_SAP) ADD_VIRTUAL_BYTECODE(ins_SAP)
				else if(instructions[i].bc == BYTECODE_AND) ADD_VIRTUAL_BYTECODE(ins_AND)
				else if(instructions[i].bc == BYTECODE_ORR) ADD_VIRTUAL_BYTECODE(ins_ORR)
				else if(instructions[i].bc == BYTECODE_NOT) ADD_VIRTUAL_BYTECODE(ins_NOT)
					
				else if(instructions[i].bc == BYTECODE_NAD) ADD_VIRTUAL_BYTECODE(ins_NAD)
				else if(instructions[i].bc == BYTECODE_NSB) ADD_VIRTUAL_BYTECODE(ins_NSB)
				else if(instructions[i].bc == BYTECODE_NMU) ADD_VIRTUAL_BYTECODE(ins_NMU)
				else if(instructions[i].bc == BYTECODE_NDV) ADD_VIRTUAL_BYTECODE(ins_NDV)
				else if(instructions[i].bc == BYTECODE_NMD) ADD_VIRTUAL_BYTECODE(ins_NMD)
				else if(instructions[i].bc == BYTECODE_NIN) ADD_VIRTUAL_BYTECODE(ins_NIN)
				else if(instructions[i].bc == BYTECODE_NDE) ADD_VIRTUAL_BYTECODE(ins_NDE)
				else if(instructions[i].bc == BYTECODE_NEQ) ADD_VIRTUAL_BYTECODE(ins_NEQ)
				else if(instructions[i].bc == BYTECODE_NGE) ADD_VIRTUAL_BYTECODE(ins_NGE)
				else if(instructions[i].bc == BYTECODE_NGT) ADD_VIRTUAL_BYTECODE(ins_NGT)
				else if(instructions[i].bc == BYTECODE_NLE) ADD_VIRTUAL_BYTECODE(ins_NLE)
				else if(instructions[i].bc == BYTECODE_NLT) ADD_VIRTUAL_BYTECODE(ins_NLT)
				else if(instructions[i].bc == BYTECODE_NNE) ADD_VIRTUAL_BYTECODE(ins_NNE)
				
				else if(instructions[i].bc == BYTECODE_ZNL) ADD_VIRTUAL_BYTECODE(ins_ZNL)
				else if(instructions[i].bc == BYTECODE_ZBO) ADD_VIRTUAL_BYTECODE(ins_ZBO)
				else if(instructions[i].bc == BYTECODE_ZNU) ADD_VIRTUAL_BYTECODE(ins_ZNU)
				else if(instructions[i].bc == BYTECODE_ZST) ADD_VIRTUAL_BYTECODE(ins_ZST)
				else if(instructions[i].bc == BYTECODE_ZAR) ADD_VIRTUAL_BYTECODE(ins_ZAR)
				else if(instructions[i].bc == BYTECODE_ZRS) ADD_VIRTUAL_BYTECODE(ins_ZRS)
				else if(instructions[i].bc == BYTECODE_ZOB) ADD_VIRTUAL_BYTECODE(ins_ZOB)
					
				else if(instructions[i].bc == BYTECODE_RTN) ADD_VIRTUAL_BYTECODE(ins_RTN)
			}
		}
	}
	
	vm->functions[function_id].virtual_ptr = r;
	
	// prepare all the constant data
	delta_vm_prepare(vm, function_id, ram);
	
	return r;
}


void delta_run_virtual(struct delta_virtual_function *f)
{
	int i;
	
	// execute virtual instructions
	for(i = 0; i < f->total_ins; ++i)
		f->ins[i].func(&f->ins[i].ins);
}

/*
 *  Delta programming language
 */

#include "delta/jit/jit_compiler.h"
#include "delta/vm/vm.h"
#include "delta/compiler/bytecode.h"
#include "delta/jit/ins.h"
#include "delta/vm/vm.h"
#include "delta/compiler/strings.h"
#include <ctype.h>
#include <assert.h>
#include <string.h>


void delta_check_permission(struct DeltaVM *vm, int i, int j, struct DeltaInstruction *instructions)
{
	// check permission access
	if(vm->functions[j].permission != DELTA_PUBLIC) {
		printf("Delta VM Runtime Error: You cannot call private/protected %s().",
			   instructions[i].func);
		exit(1);
	}
}


void delta_check_static(struct DeltaVM *vm, int i, int j, struct DeltaInstruction *instructions)
{
	// check static
	if(delta_strpos(instructions[i].func, ".") >= 0) {
		if(toupper(instructions[i].func[0]) == instructions[i].func[0]) {
			// we are calling a method with a class name, so the function
			// must be static
			if(!vm->functions[j].is_static) {
				printf("Delta VM Runtime Error: You cannot call non-static %s() as if it were static.",
					   instructions[i].func);
				exit(1);
			}
		}
	}
}


delta_jit_function delta_compile_jit(struct DeltaVM *c, char *function_name)
{
	int i, j, loop_id = 0, end = 0, function_id = -1, k;
	struct DeltaInstruction *instructions = NULL;
	
#if 0
	for(i = 0; i < c->total_functions; ++i) {
		for(j = 0; j < c->functions[i].total_ins; ++j) {
			// show bytecode
			printf("J1 {%d} %d:", i, c->functions[i].ins[j].bc);
			for(k = 0; k < c->functions[i].ins[j].args; ++k)
				printf(" %d", c->functions[i].ins[j].arg[k]);
			printf("\n");
		}
	}
#endif
	
	// try to find the function
	for(i = 0; i < c->total_functions; ++i) {
		if(!strcmp(function_name, c->functions[i].name)) {
			// found it, if the function is already JIT compiled then great we saved some time
			if(c->functions[i].jit_ptr != NULL)
				return c->functions[i].jit_ptr;
			
			instructions = c->functions[i].ins;
			end = c->functions[i].total_ins;
			function_id = i;
			
			// calculate the required ram
			c->functions[i].total_ram = delta_calculate_total_ram(&c->functions[i]);
			c->functions[i].total_static_ram = delta_calculate_total_static_ram(&c->functions[i]);
			
			break;
		}
	}
	assert(function_id >= 0);
	
	// because functions cannot be recursivly compiled, JIT_V0 etc would be overriden we first
	// take a quick look through the instructions and compile any required functions first
	for(i = 0; i < end; ++i) {
		if(instructions[i].bc == BYTECODE_CAL) {
			// check if it is a user function
			for(j = 0; j < c->total_functions; ++j) {
				if(!strcmp(c->functions[j].name, instructions[i].func)) {
					// FIXME: dont compile yourself
					c->functions[j].jit_ptr = delta_compile_jit(c, instructions[i].func);
					break;
				}
			}
		}
	}
	
	// prepare for JIT
	jit_insn **loop = (jit_insn**) calloc(16, sizeof(jit_insn*));
	jit_insn *codeBuffer = (jit_insn*) calloc(1000, sizeof(jit_insn));
	delta_jit_function f = (delta_jit_function) (jit_set_ip(codeBuffer).vptr);
	
	// create a new RAM
	struct DeltaVariable **ram = (struct DeltaVariable**)
		calloc(c->functions[function_id].total_ram, sizeof(struct DeltaVariable*));
	for(i = 0; i < c->functions[function_id].total_ram; ++i)
		ram[i] = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable));
	
	// static RAM for interfunction communications
	static struct DeltaVariable **static_ram = NULL;
	if(static_ram == NULL) {
		static_ram = (struct DeltaVariable**)
			calloc(c->functions[function_id].total_static_ram, sizeof(struct DeltaVariable*));
		for(i = 0; i < c->functions[function_id].total_static_ram; ++i)
			static_ram[i] = (struct DeltaVariable*) malloc(sizeof(struct DeltaVariable));
	}
	
	// TODO: this will have to be changed for multithreaded apps, for now we'll just add the ram to
	// the function
	c->functions[function_id].ram = ram;
	
	// I think we must have this
	jit_leaf(0);
	
	// compile instructions
	for(i = 0; i < end; ++i) {
#if 0
		// show bytecode
		printf("JC {%d} %d:", function_id, instructions[i].bc);
		for(j = 0; j < instructions[i].args; ++j)
			printf(" %d", instructions[i].arg[j]);
		printf("\n");
#endif
		
		// skip NUL bytecodes
		if(instructions[i].bc == BYTECODE_NUL)
			continue;
		
		if(instructions[i].bc == BYTECODE_IFS) {
			// copy in the boolean argument as a double
			jit_ldi_d(JIT_R0, &ram[instructions[i].arg[1]]->value.number);
			jit_truncr_d_i(JIT_R0, JIT_R0);
			//printf("*** IFS %d\n", instructions[i].arg[0]);
			loop[instructions[i].arg[0]] = jit_beqi_i(jit_forward(), JIT_R0, 0);
		}
		else if(instructions[i].bc == BYTECODE_LOP) {
			// copy in the boolean argument as a double
			jit_ldi_d(JIT_R0, &ram[instructions[i].arg[1]]->value.number);
			jit_truncr_d_i(JIT_R0, JIT_R0);
			//printf("*** LOP %d\n", instructions[i].arg[0]);
			loop[instructions[i].arg[0]] = jit_beqi_i(jit_forward(), JIT_R0, 0);
		}
		else if(instructions[i].bc == BYTECODE_LBL) {
			//printf("*** LBL %d\n", instructions[i].arg[0]);
			loop[instructions[i].arg[0]] = jit_get_label();
		}
		else if(instructions[i].bc == BYTECODE_GTO) {
			//printf("*** GTO %d\n", instructions[i].arg[0]);
			jit_jmpi(loop[instructions[i].arg[0]]);
		}
		else if(instructions[i].bc == BYTECODE_PAT) {
			//printf("*** PAT %d\n", instructions[i].arg[0]);
			jit_patch(loop[instructions[i].arg[0]]);
		}
		else if(instructions[i].bc == BYTECODE_JMP) {
			//printf("*** JMP %d\n", instructions[i].arg[0]);
			jit_movi_i(JIT_R0, 1);
			loop[instructions[i].arg[0]] = jit_beqi_i(jit_forward(), JIT_R0, 1);
		}
		else if(instructions[i].bc == BYTECODE_RTN) {
			jit_ret();
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
			
			// link function call
			jit_movi_p(JIT_V0, &instructions[i]);
			jit_prepare(1);
			jit_pusharg_p(JIT_V0);
			
			// link the function by its name
			if(instructions[i].bc == BYTECODE_CAL) {
				delta_jit_function linked = NULL;
				int fargs = (instructions[i].args - 1) / 2;
				
				// built-in functions
				for(j = 0; j < c->total_delta_functions; ++j) {
					if(!stricmp(c->delta_functions[j]->name, instructions[i].func) &&
					   fargs >= c->delta_functions[j]->min_args &&
					   fargs <= c->delta_functions[j]->max_args) {
						delta_check_static(c, i, j, instructions);
						linked = c->delta_functions[j]->function_ptr;
						break;
					}
				}
				
				// before the actual compiled function can be called we need to call a joining
				// function that takes care of things like stack backtrace and preparing function
				// arguments
				if(linked == NULL) {
					jit_finish(ins_CAL);
					jit_movi_p(JIT_V0, &instructions[i]);
					jit_prepare(1);
					jit_pusharg_p(JIT_V0);
				}
				
				// if the function could not be found, then maybe its a user function
				if(linked == NULL) {
					for(j = 0; j < c->total_functions; ++j) {
						if(!stricmp(c->functions[j].name, instructions[i].func)) {
							delta_check_permission(c, i, j, instructions);
							delta_check_static(c, i, j, instructions);
							linked = c->functions[j].jit_ptr;
							break;
						}
					}
				}
				
				if(linked == NULL) {
					printf("Delta VM Runtime Error: Cannot link function '%s' (bytecode = 0x%x)\n",
						   instructions[i].func, instructions[i].bc);
					//printf("Candidates (%d) are:\n", c->total_delta_functions);
					//for(j = 0; j < c->total_delta_functions; ++i)
					//	printf("  %s\n", c->delta_functions[i]->name);
					exit(1);
				}
				
				jit_finish(linked);
			}
			else {
				// mostly operator instructions
					 if(0) { }
				else if(instructions[i].bc == BYTECODE_ADD) jit_finish(ins_ADD);
				else if(instructions[i].bc == BYTECODE_AG1) jit_finish(ins_AG1);
				else if(instructions[i].bc == BYTECODE_ARG) jit_finish(ins_ARG);
				else if(instructions[i].bc == BYTECODE_AS1) jit_finish(ins_AS1);
				else if(instructions[i].bc == BYTECODE_CEQ) jit_finish(ins_CEQ);
				else if(instructions[i].bc == BYTECODE_CGE) jit_finish(ins_CGE);
				else if(instructions[i].bc == BYTECODE_CGT) jit_finish(ins_CGT);
				else if(instructions[i].bc == BYTECODE_CLE) jit_finish(ins_CLE);
				else if(instructions[i].bc == BYTECODE_CLT) jit_finish(ins_CLT);
				else if(instructions[i].bc == BYTECODE_CNE) jit_finish(ins_CNE);
				else if(instructions[i].bc == BYTECODE_DEC) jit_finish(ins_DEC);
				else if(instructions[i].bc == BYTECODE_DIV) jit_finish(ins_DIV);
				else if(instructions[i].bc == BYTECODE_INC) jit_finish(ins_INC);
				else if(instructions[i].bc == BYTECODE_MOD) jit_finish(ins_MOD);
				else if(instructions[i].bc == BYTECODE_MUL) jit_finish(ins_MUL);
				else if(instructions[i].bc == BYTECODE_SET) jit_finish(ins_SET);
				else if(instructions[i].bc == BYTECODE_SUB) jit_finish(ins_SUB);
				else if(instructions[i].bc == BYTECODE_SAP) jit_finish(ins_SAP);
				else if(instructions[i].bc == BYTECODE_AND) jit_finish(ins_AND);
				else if(instructions[i].bc == BYTECODE_ORR) jit_finish(ins_ORR);
				else if(instructions[i].bc == BYTECODE_NOT) jit_finish(ins_NOT);
				
				else if(instructions[i].bc == BYTECODE_NAD) jit_finish(ins_NAD);
				else if(instructions[i].bc == BYTECODE_NSB) jit_finish(ins_NSB);
				else if(instructions[i].bc == BYTECODE_NMU) jit_finish(ins_NMU);
				else if(instructions[i].bc == BYTECODE_NDV) jit_finish(ins_NDV);
				else if(instructions[i].bc == BYTECODE_NMD) jit_finish(ins_NMD);
				else if(instructions[i].bc == BYTECODE_NIN) jit_finish(ins_NIN);
				else if(instructions[i].bc == BYTECODE_NDE) jit_finish(ins_NDE);
				else if(instructions[i].bc == BYTECODE_NEQ) jit_finish(ins_NEQ);
				else if(instructions[i].bc == BYTECODE_NGE) jit_finish(ins_NGE);
				else if(instructions[i].bc == BYTECODE_NGT) jit_finish(ins_NGT);
				else if(instructions[i].bc == BYTECODE_NLE) jit_finish(ins_NLE);
				else if(instructions[i].bc == BYTECODE_NLT) jit_finish(ins_NLT);
				else if(instructions[i].bc == BYTECODE_NNE) jit_finish(ins_NNE);
				
				else if(instructions[i].bc == BYTECODE_ZNL) jit_finish(ins_ZNL);
				else if(instructions[i].bc == BYTECODE_ZBO) jit_finish(ins_ZBO);
				else if(instructions[i].bc == BYTECODE_ZNU) jit_finish(ins_ZNU);
				else if(instructions[i].bc == BYTECODE_ZST) jit_finish(ins_ZST);
				else if(instructions[i].bc == BYTECODE_ZAR) jit_finish(ins_ZAR);
				else if(instructions[i].bc == BYTECODE_ZRS) jit_finish(ins_ZRS);
				else if(instructions[i].bc == BYTECODE_ZOB) jit_finish(ins_ZOB);
				
				else if(instructions[i].bc == BYTECODE_OST) jit_finish(ins_OST);
				else if(instructions[i].bc == BYTECODE_OGT) jit_finish(ins_OGT);
				else if(instructions[i].bc == BYTECODE_MET) jit_finish(ins_MET);
				
				else                                        jit_finish(ins_NUL);
			}
		}
	}
	
	// this is never reached as delta functions will always have a BYTECODE_RTN at the end, but I
	// will keep this here anyway because if it doesn't occur it causes very bad memory issues.
	jit_ret();
	
	// all done
	jit_flush_code(codeBuffer, jit_get_ip().ptr);
	c->functions[function_id].jit_ptr = f;
	
	// prepare all the constant data
	delta_vm_prepare(c, function_id, ram);
	
	return f;
}

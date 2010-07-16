/*
 *  Delta programming language
 */

#include "delta/jit/jit_compiler.h"
#include "delta/vm/vm.h"
#include "delta/compiler/bytecode.h"
#include "delta/jit/ins.h"
#include <string.h>


static jit_insn codeBuffer[1024];


stack_function delta_compile_jit(struct DeltaCompiler *c, int at, int end)
{
	// pointer to generated code
	stack_function f = (stack_function) (jit_set_ip(codeBuffer).vptr);
	int i, loop_id = 0, j;
	jit_insn **loop = (jit_insn**) calloc(16, sizeof(jit_insn*));
	struct DeltaInstruction* instructions = c->ins;
	
	// take an argument we wont use
	jit_leaf(0);
	
	// compile instructions
	for(i = at; i < end; ++i) {
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
		} else {
			// link argument addresses
			instructions[i].varg = (struct DeltaVariable**)
				malloc(instructions[i].args * sizeof(struct DeltaVariable*));
			for(j = 0; j < instructions[i].args; ++j)
				instructions[i].varg[j] = ram[instructions[i].arg[j]];
			
			// link function call
			jit_movi_p(JIT_V0, &instructions[i]);
			jit_prepare(1);
			jit_pusharg_p(JIT_V0);
			
			// link the function by its name
			if(instructions[i].func != NULL) {
				int linked = -1;
				for(j = 0; j < total_delta_functions; ++j) {
					if(!strcmp(delta_functions[j]->name, instructions[i].func)) {
						// TODO: check argument count is with acceptable range
						linked = j;
						break;
					}
				}
				
				if(linked < 0) {
					printf("Delta VM Runtime Error: Cannot link function '%s' (bytecode = 0x%x)\n",
						   instructions[i].func, instructions[i].bc);
					exit(1);
				}
				
				jit_finish(delta_functions[linked]->function_ptr);
			}
			else {
				// mostly operator instructions
					 if(0) { }
				else if(instructions[i].bc == BYTECODE_ADD) jit_finish(ins_ADD);
				else if(instructions[i].bc == BYTECODE_AG1) jit_finish(ins_AG1);
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
				else if(instructions[i].bc == BYTECODE_RTN) jit_finish(ins_RTN);
				else if(instructions[i].bc == BYTECODE_SET) jit_finish(ins_SET);
				else if(instructions[i].bc == BYTECODE_SUB) jit_finish(ins_SUB);
				else if(instructions[i].bc == BYTECODE_SAP) jit_finish(ins_SAP);
				else                                        jit_finish(ins_NUL);
			}
		}
	}
	jit_ret();
	
	// all done
	jit_flush_code(codeBuffer, jit_get_ip().ptr);
	return f;
}

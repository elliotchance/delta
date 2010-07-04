/*
 *  Delta programming language
 */

#include "jit_compiler.h"
#include "vm.h"
#include "bytecode.h"
#include "ins.h"
#include <string.h>

jit_insn codeBuffer[1024];

stack_function delta_compile_jit(DeltaCompiler *c, int start, int end)
{
	// pointer to generated code
	stack_function f = (stack_function) (jit_set_ip(codeBuffer).vptr);
	int i, loop_id = 0;
	jit_insn **loop = (jit_insn**) calloc(16, sizeof(jit_insn*));
	struct DeltaInstruction* instructions = c->ins;
	
	// take an argument we wont use
	jit_leaf(0);
	
	// compile instructions
	for(i = start; i < end; ++i) {
		if(instructions[i].bc == BYTECODE_IFS) {
			// copy in the boolean argument
			//jit_ldi_d(JIT_R0, &ram[instructions[i].arg[1]]->value.number);
			//jit_movi_d(JIT_R1, 0);
			//jit_bner_d(loop, JIT_R0, JIT_R1);
			
			// copy in the boolean argument as a double
			jit_ldi_d(JIT_R0, &ram[instructions[i].arg[1]]->value.number);
			jit_truncr_d_i(JIT_R0, JIT_R0);
			loop[0] = jit_bnei_i(jit_forward(), JIT_R0, 0);
		}
		else if(instructions[i].bc == BYTECODE_LOP) {
			// copy in the boolean argument as a double
			jit_ldi_d(JIT_R0, &ram[instructions[i].arg[1]]->value.number);
			jit_truncr_d_i(JIT_R0, JIT_R0);
			loop[instructions[i].arg[0]] = jit_beqi_i(jit_forward(), JIT_R0, 0);
		}
		else if(instructions[i].bc == BYTECODE_LBL)
			loop[instructions[i].arg[1]] = jit_get_label();
		else if(instructions[i].bc == BYTECODE_GTO)
			jit_jmpi(loop[instructions[i].arg[1]]);
		else if(instructions[i].bc == BYTECODE_PAT) {
			jit_patch(loop[instructions[i].arg[0]]);
		} else if(instructions[i].bc == BYTECODE_JMP) {
			jit_movi_i(JIT_R0, 1);
			loop[1] = jit_beqi_i(jit_forward(), JIT_R0, 1);
		} else {
			jit_movi_p(JIT_R0, &instructions[i]);
			jit_prepare(1);
			jit_pusharg_p(JIT_R0);
			
			// link the function by its name
			if(instructions[i].func != NULL) {
				int j, linked = -1;
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
				     if(instructions[i].bc == BYTECODE_ADD) jit_finish(ins_ADD);
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
				else if(instructions[i].bc == BYTECODE_GTO) jit_finish(ins_GTO);
				else if(instructions[i].bc == BYTECODE_INC) jit_finish(ins_INC);
				else if(instructions[i].bc == BYTECODE_MUL) jit_finish(ins_MUL);
				else if(instructions[i].bc == BYTECODE_RTN) jit_finish(ins_RTN);
				else if(instructions[i].bc == BYTECODE_SET) jit_finish(ins_SET);
				else if(instructions[i].bc == BYTECODE_SUB) jit_finish(ins_SUB);
				else                                        jit_finish(ins_NUL);
			}
		}
	}
	jit_ret();
	
	// all done
	jit_flush_code(codeBuffer, jit_get_ip().ptr);
	return f;
}

/*
 *  jit_compiler.c
 *  jit
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#include "jit_compiler.h"
#include "vm.h"
#include "bytecode.h"
#include "ins.h"

jit_insn codeBuffer[1024];

stack_function delta_compile_jit(DeltaCompiler *c, int start, int end)
{
	// pointer to generated code
	stack_function f = (stack_function) (jit_set_ip(codeBuffer).vptr);
	int i;
	jit_insn *loop;
	struct DeltaInstruction* instructions = c->ins;
	
	// take an argument we wont use
	jit_leaf(0);
	
	// compile instructions
	for(i = start; i < end; ++i) {
		if(instructions[i].bc == BYTECODE_IFS) {
			// copy in the boolean argument
			jit_ldi_d(JIT_R0, &ram[instructions[i].arg[1]].value.number);
			jit_movi_d(JIT_R1, 0);
			jit_bner_d(loop, JIT_R0, JIT_R1);
		}
		else if(instructions[i].bc == BYTECODE_LBL) {
			loop = jit_get_label();
		} else {
			jit_movi_p(JIT_R0, &instructions[i]);
			jit_prepare(1);
			jit_pusharg_p(JIT_R0);
			
			// these instructions are guaranteed to not change the stack position
			     if(instructions[i].bc == BYTECODE_ADD) jit_finish(ins_ADD);
			else if(instructions[i].bc == BYTECODE_APH) jit_finish(ins_APH);
			else if(instructions[i].bc == BYTECODE_CEQ) jit_finish(ins_CEQ);
			else if(instructions[i].bc == BYTECODE_CGE) jit_finish(ins_CGE);
			else if(instructions[i].bc == BYTECODE_CGT) jit_finish(ins_CGT);
			else if(instructions[i].bc == BYTECODE_CLE) jit_finish(ins_CLE);
			else if(instructions[i].bc == BYTECODE_CLT) jit_finish(ins_CLT);
			else if(instructions[i].bc == BYTECODE_CNE) jit_finish(ins_CNE);
			else if(instructions[i].bc == BYTECODE_COS) jit_finish(ins_COS);
			else if(instructions[i].bc == BYTECODE_DEC) jit_finish(ins_DEC);
			else if(instructions[i].bc == BYTECODE_DIV) jit_finish(ins_DIV);
			else if(instructions[i].bc == BYTECODE_GTO) jit_finish(ins_GTO);
			else if(instructions[i].bc == BYTECODE_INC) jit_finish(ins_INC);
			else if(instructions[i].bc == BYTECODE_MUL) jit_finish(ins_MUL);
			else if(instructions[i].bc == BYTECODE_OUT) jit_finish(ins_OUT);
			else if(instructions[i].bc == BYTECODE_RTN) jit_finish(ins_RTN);
			else if(instructions[i].bc == BYTECODE_SET) jit_finish(ins_SET);
			else if(instructions[i].bc == BYTECODE_SIN) jit_finish(ins_SIN);
			else if(instructions[i].bc == BYTECODE_SQT) jit_finish(ins_SQT);
			else if(instructions[i].bc == BYTECODE_SUB) jit_finish(ins_SUB);
			else if(instructions[i].bc == BYTECODE_TAN) jit_finish(ins_TAN);
			else                                        jit_finish(ins_NUL);
		}
	}
	jit_ret();
	
	// all done
	jit_flush_code(codeBuffer, jit_get_ip().ptr);
	return f;
}

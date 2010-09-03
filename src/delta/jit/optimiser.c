/*
 *  Delta programming language
 */

#include "optimiser.h"
#include "delta/compiler/bytecode.h"
#include "delta/macros.h"


int delta_is_math_bytecode(DeltaByteCode bc)
{
	if(bc == BYTECODE_ADD || bc == BYTECODE_SUB || bc == BYTECODE_MUL || bc == BYTECODE_DIV ||
	   bc == BYTECODE_MOD || bc == BYTECODE_INC || bc == BYTECODE_DEC)
		return 1;
	return 0;
}


int delta_is_compare_bytecode(DeltaByteCode bc)
{
	if(bc == BYTECODE_CEQ || bc == BYTECODE_CNE || bc == BYTECODE_CLT || bc == BYTECODE_CGT ||
	   bc == BYTECODE_CLE || bc == BYTECODE_CGE)
		return 1;
	return 0;
}


int delta_is_temp(struct DeltaCompiler *c, int function_id, int address)
{
	int i;
	for(i = 0; i < c->functions[function_id].total_ins; ++i) {
		if(c->functions[function_id].ins[i].args > 0 &&
		   c->functions[function_id].ins[i].arg[0] == address)
			return DELTA_YES;
	}
	return DELTA_NO;
}


int delta_translate_safe_bytecode(int bc)
{
	if(bc == BYTECODE_ADD)
		return BYTECODE_NAD;
	if(bc == BYTECODE_SUB)
		return BYTECODE_NSB;
	if(bc == BYTECODE_MUL)
		return BYTECODE_NMU;
	if(bc == BYTECODE_DIV)
		return BYTECODE_NDV;
	if(bc == BYTECODE_MOD)
		return BYTECODE_NMD;
	if(bc == BYTECODE_INC)
		return BYTECODE_NIN;
	if(bc == BYTECODE_DEC)
		return BYTECODE_NDE;
	
	if(bc == BYTECODE_CEQ)
		return BYTECODE_NEQ;
	if(bc == BYTECODE_CNE)
		return BYTECODE_NNE;
	if(bc == BYTECODE_CLT)
		return BYTECODE_NLT;
	if(bc == BYTECODE_CLE)
		return BYTECODE_NLE;
	if(bc == BYTECODE_CGT)
		return BYTECODE_NGT;
	if(bc == BYTECODE_CGE)
		return BYTECODE_NGE;
	
	// no equivilent fastmath operation
	return bc;
}


void delta_optimise_bytecode(struct DeltaCompiler *c, int function_id, int at, int end)
{
	int i, dest = 0, j, k;
	
	struct DeltaInstruction* ins = c->functions[function_id].ins;
	printf("\nBefore:\n");
	for(i = at; i < end; ++i) {
		printf("  id = %d, bc = %d |", i, ins[i].bc);
		for(j = 0; j < ins[i].args; ++j)
			printf(" %d", ins[i].arg[j]);
		printf("\n");
	}
	
	// Phase 1: Any unwanted bytecodes are marked as BYTECODE_NUL
	for(i = at; i < end; ++i) {
		if(ins[i].bc == BYTECODE_ADD && ins[i + 1].bc == BYTECODE_SET) {
			// if these instructions are using a certain pattern of addresses then the bytecodes
			// can be translated into a single instruction
			if(ins[i].arg[0] == ins[i + 1].arg[1]) {
				// recognise incrementing
				int found_inc = 0;
				
				for(j = 0; j < c->functions[function_id].total_constants; ++j) {
					if(c->functions[function_id].constants[j].ram_location == ins[i].arg[2]) {
						if(c->functions[function_id].constants[j].type == DELTA_TYPE_NUMBER &&
						   c->functions[function_id].constants[j].value.number == 1.0)
							found_inc = 1;
						break;
					}
				}
				
				if(found_inc)
					ins[i].bc = BYTECODE_INC;
				
				ins[i].arg[0] = ins[i + 1].arg[0];
				ins[i + 1].bc = BYTECODE_NUL;
			}
		}
	}
	
	// Phase 2: Try to determine safe types for variables
	printf("\nPhase 2:\n");
	for(j = 0; j < c->functions[function_id].total_vars; ++j) {
		//printf("Resolving %s... ", c->vars[j].name);
		int is_numerical = 1;
		
		for(i = at; i < end; ++i) {
			//int found = 0;
			for(k = 0; k < ins[i].args; ++k) {
				if(ins[i].arg[k] == c->functions[function_id].vars[j].ram_location) {
					//found = 1;
					break;
				}
			}
			
			// check for numerical operation
			int numerical = 0;
			
			// TODO: set is only allowed is the argument is a numerical constant
			if(ins[i].bc == BYTECODE_SET)
				numerical = 1;
			
			// these are control flow and won't affect the data type
			if(ins[i].bc >= BYTECODE_LBL)
				numerical = 1;
			
			// simple math
			if(delta_is_math_bytecode(ins[i].bc))
				numerical = 1;
			
			if(delta_is_compare_bytecode(ins[i].bc))
				numerical = 1;
			
			// misc
			if(ins[i].bc == BYTECODE_CAL || ins[i].bc == BYTECODE_NUL)
				numerical = 1;
				
			is_numerical = is_numerical && numerical;
		}
		
		if(is_numerical)
			c->functions[function_id].vars[j].safe_type = DELTA_TYPE_NUMBER;
		else
			c->functions[function_id].vars[j].safe_type = DELTA_TYPE_UNKNOWN;
	}
	
	// Phase 3: Use the safe type to resolve numerical only operations
	for(i = at; i < end; ++i) {
		// onlt care about possible math operations
		if(delta_is_math_bytecode(ins[i].bc) || delta_is_compare_bytecode(ins[i].bc)) {
			printf("> [%d]", ins[i].bc);
			int is_safe = 1;
			
			for(j = 0; j < ins[i].args; ++j) {
				// all of the arguments must be proven numerical only
				int found = 0;
				
				// from a variable
				for(k = 0; k < c->functions[function_id].total_vars; ++k) {
					if(c->functions[function_id].vars[k].ram_location == ins[i].arg[j] &&
					   c->functions[function_id].vars[k].safe_type == DELTA_TYPE_NUMBER) {
						found = 1;
						break;
					}
				}
				
				// from a constant
				for(k = 0; k < c->functions[function_id].total_constants; ++k) {
					if(c->functions[function_id].constants[k].ram_location == ins[i].arg[j] &&
					   c->functions[function_id].constants[k].type == DELTA_TYPE_NUMBER) {
						found = 1;
						break;
					}
				}
				
				// if its a temp variable then it will return a set type, because we are only
				// looking at numerical operation we know that the temp value returned will always
				// be a NUMBER
				if(!found && !delta_is_temp(c, function_id, ins[i].arg[j])) {
					is_safe = 0;
					printf(" %d", ins[i].arg[j]);
					break;
				}
				else
					printf(" (%d)", ins[i].arg[j]);
			}
			
			if(is_safe) {
				int new_bc = delta_translate_safe_bytecode(ins[i].bc);
				printf(" SAFE %d -> %d\n", ins[i].bc, new_bc);
				ins[i].bc = new_bc;
			}
			else
				printf(" UNSAFE\n");
		}
	}
	
	printf("\nAfter:\n");
	for(i = at; i < end; ++i) {
		printf("  id = %d, bc = %d |", i, ins[i].bc);
		for(j = 0; j < ins[i].args; ++j)
			printf(" %d", ins[i].arg[j]);
		printf("\n");
	}
	
	//exit(0);
}

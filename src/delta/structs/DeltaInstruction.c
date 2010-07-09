/*
 *  Delta programming language
 */

#include "DeltaInstruction.h"
#include "compiler.h"


struct DeltaInstruction new_DeltaInstruction0(char *name, DeltaByteCode bc)
{
	struct DeltaInstruction d;
	d.func = name;
	d.bc = bc;
	
	return d;
}


struct DeltaInstruction new_DeltaInstruction1(char *name, DeltaByteCode bc, int destination)
{
	struct DeltaInstruction d;
	d.func = name;
	d.bc = bc;
	d.args = 1;
	d.arg = (int*) malloc(d.args * sizeof(int));
	d.arg[0] = destination;
	
	return d;
}


struct DeltaInstruction new_DeltaInstruction2(char *name, DeltaByteCode bc, int destination, int source1)
{
	struct DeltaInstruction d;
	d.func = name;
	d.bc = bc;
	d.args = 2;
	d.arg = (int*) malloc(d.args * sizeof(int));
	d.arg[0] = destination;
	d.arg[1] = source1;
	
	return d;
}


struct DeltaInstruction new_DeltaInstruction3(char *name, DeltaByteCode bc, int destination, int source1, int source2)
{
	struct DeltaInstruction d;
	d.func = name;
	d.bc = bc;
	d.args = 3;
	d.arg = (int*) malloc(d.args * sizeof(int));
	d.arg[0] = destination;
	d.arg[1] = source1;
	d.arg[2] = source2;
	
	return d;
}


struct DeltaInstruction new_DeltaInstructionN(char *name, DeltaByteCode bc)
{
	struct DeltaInstruction d;
	d.func = name;
	d.bc = bc;
	
	// we don't just assign the pointer we copy the data so that theres no danger of the vaues
	// changing later and also it saves space.
	d.args = arg_count[arg_depth];
	d.arg = (int*) calloc(d.args, sizeof(int));
	int i;
	for(i = 0; i < d.args; ++i)
		d.arg[i] = arg_ptr[arg_depth][i];
	
	return d;
}

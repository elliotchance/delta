/*
 *  Delta programming language
 */

#include "delta/structs.h"

struct DeltaInstruction new_DeltaInstruction0(char *name, DeltaByteCode bc);
struct DeltaInstruction new_DeltaInstruction1(char *name, DeltaByteCode bc, int destination);
struct DeltaInstruction new_DeltaInstruction2(char *name, DeltaByteCode bc, int destination,
											  int source1);
struct DeltaInstruction new_DeltaInstruction3(char *name, DeltaByteCode bc, int destination,
											  int source1, int source2);
struct DeltaInstruction new_DeltaInstructionN(char *name, DeltaByteCode bc);
void                    free_DeltaInstructon(struct DeltaInstruction *ins);

/*
 *  Delta programming language
 */

#include "save.h"
#include "../macros.h"


int delta_save_file(struct DeltaCompiler *c, const char* out_file)
{
	// prepare
	FILE *f = fopen(out_file, "w");
	int i;
	
	// write instructions to output file
	//for(i = 0; i < c->total_ins; ++i)
	//	fwrite((const void*) &c->ins[i], sizeof(struct DeltaInstruction), 1, f);
	
	// clean up
	fclose(f);
	return DELTA_SUCCESS;
}

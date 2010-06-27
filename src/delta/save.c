/*
 *  save.c
 *  jit
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#include "save.h"


int delta_save_file(DeltaCompiler *c, const char* out_file)
{
	// prepare
	FILE *f = fopen(out_file, "w");
	int i;
	
	// write instructions to output file
	for(i = 0; i < c->total_ins; ++i)
		fwrite((const void*) &c->ins[i], sizeof(DI), 1, f);
	
	// clean up
	fclose(f);
	return DELTA_SUCCESS;
}

/*
 *  Delta programming language
 */


#include "save.h"
#include "../macros.h"
#include "../compiler/bytecode.h"
#include <assert.h>
#include <sys/stat.h>
#include <utime.h>


int delta_write_string(FILE *fp, char *str)
{
	if(str == NULL) {
		delta_write_int(fp, 0);
		return 1;
	}
	
	delta_write_int(fp, strlen(str));
	fwrite(str, strlen(str), 1, fp);
	
	return 1;
}


int delta_write_int(FILE *fp, int value)
{
	fwrite((const void*) &value, sizeof(value), 1, fp);
	return 1;
}


int delta_write_double(FILE *fp, double value)
{
	fwrite((const void*) &value, sizeof(value), 1, fp);
	return 1;
}


int delta_write_bytecode(FILE *fp, DeltaByteCode bc)
{
	fwrite((const void*) &bc, sizeof(bc), 1, fp);
	return 1;
}


int delta_save_file(struct DeltaCompiler *c, const char* out_file, char* orig)
{
	// prepare
	FILE *f = fopen(out_file, "w");
	int i, j, k;
	
	// write total functions
	delta_write_int(f, c->total_functions);
	
	// write functions to output file
	for(i = 0; i < c->total_functions; ++i) {
		delta_write_string(f, c->functions[i].name);
		
		// instructions
		delta_write_int(f, c->functions[i].total_ins);
		for(j = 0; j < c->functions[i].total_ins; ++j) {
			delta_write_string(f, c->functions[i].ins[j].func);
			delta_write_bytecode(f, c->functions[i].ins[j].bc);
			
			delta_write_int(f, c->functions[i].ins[j].args);
			for(k = 0; k < c->functions[i].ins[j].args; ++k)
				delta_write_int(f, c->functions[i].ins[j].arg[k]);
		}
		
		// variables / RAM
		delta_write_int(f, c->functions[i].total_vars);
		
		// labels
		delta_write_int(f, c->functions[i].total_labels);
		
		// constants
		delta_write_int(f, c->functions[i].total_constants);
		for(j = 0; j < c->functions[i].total_constants; ++j) {
			//delta_write_string(f, c->functions[i].constants[j].name);
			
			DeltaVariableType type = c->functions[i].constants[j].type;
			delta_write_int(f, type);
			if(type == DELTA_TYPE_NULL) {
				// do nothing
			}
			else if(type == DELTA_TYPE_NUMBER) {
				delta_write_double(f, c->functions[i].constants[j].value.number);
			}
			else if(type == DELTA_TYPE_STRING) {
				delta_write_int(f, c->functions[i].constants[j].size);
				fwrite(c->functions[i].constants[j].value.ptr, c->functions[i].constants[j].size, 1,
					   f);
			}
			else
				assert(0);
			
			delta_write_int(f, c->functions[i].constants[j].ram_location);
			delta_write_int(f, c->functions[i].constants[j].size);
		}
	}
	
	// clean up
	fclose(f);
	
	// update mtime
	if(orig != NULL) {
		struct stat stat1;
		stat(orig, &stat1);
		
		struct utimbuf stat2;
		stat2.actime = stat1.st_atime;
		stat2.modtime = stat1.st_mtime;
		
		utime(out_file, &stat2);
	}
	
	return DELTA_SUCCESS;
}

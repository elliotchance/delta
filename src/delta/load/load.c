/*
 *  Delta programming language
 */


#include "load.h"
#include "delta/vm/vm.h"


char* delta_read_string(FILE *fp)
{
	char *str = (char*) malloc(1024);
	int i = 0;
	
	while(1) {
		int ch = fgetc(fp);
		str[i++] = (char) ch;
		if(ch == '\0')
			break;
	}
	
	return str;
}


int delta_read_int(FILE *fp)
{
	int temp;
	fread((void*) &temp, sizeof(temp), 1, fp);
	return temp;
}


double delta_read_double(FILE *fp)
{
	double temp;
	fread((void*) &temp, sizeof(temp), 1, fp);
	return temp;
}


DeltaByteCode delta_read_bytecode(FILE *fp)
{
	DeltaByteCode temp;
	fread((void*) &temp, sizeof(temp), 1, fp);
	return temp;
}


struct DeltaVM* delta_load_file(const char* in_file)
{
	// prepare
	FILE *f = fopen(in_file, "r");
	int i, j, k;
	struct DeltaVM *vm = delta_vm_init();
	
	// read total functions
	vm->total_functions = delta_read_int(f);
	vm->functions = (struct DeltaCompiledFunction*)
		calloc(vm->total_functions, sizeof(struct DeltaCompiledFunction));
	printf("Loading %d functions\n", vm->total_functions);
	
	// write functions to output file
	for(i = 0; i < vm->total_functions; ++i) {
		vm->functions[i].name = delta_read_string(f);
		printf("  %s", vm->functions[i].name);
		
		// instructions
		vm->functions[i].alloc_ins = vm->functions[i].total_ins = delta_read_int(f);
		vm->functions[i].ins = (struct DeltaInstruction*)
			calloc(vm->functions[i].alloc_ins, sizeof(struct DeltaInstruction));
		for(j = 0; j < vm->functions[i].total_ins; ++j) {
			vm->functions[i].ins[j].func = delta_read_string(f);
			vm->functions[i].ins[j].bc = delta_read_bytecode(f);
			
			vm->functions[i].ins[j].args = delta_read_int(f);
			vm->functions[i].ins[j].arg = (int*) calloc(vm->functions[i].ins[j].args, sizeof(int));
			for(k = 0; k < vm->functions[i].ins[j].args; ++k)
				vm->functions[i].ins[j].arg[k] = delta_read_int(f);
		}
		printf(" -> %d ins\n", vm->functions[i].alloc_ins);
		
		// variables / RAM
		vm->functions[i].alloc_vars = vm->functions[i].total_vars = delta_read_int(f);
		vm->functions[i].vars = (struct DeltaVariable*)
			calloc(vm->functions[i].alloc_vars, sizeof(struct DeltaVariable));
		printf("RAM = %d\n", vm->functions[i].alloc_vars);
		
		// labels
		vm->functions[i].alloc_labels = vm->functions[i].total_labels = delta_read_int(f);
		vm->functions[i].labels = (struct DeltaLabel*)
			calloc(vm->functions[i].alloc_labels, sizeof(struct DeltaLabel));
		
		// constants
		vm->functions[i].alloc_constants = vm->functions[i].total_constants = delta_read_int(f);
		vm->functions[i].constants = (struct DeltaVariable*)
			calloc(vm->functions[i].alloc_constants, sizeof(struct DeltaVariable));
		for(j = 0; j < vm->functions[i].total_constants; ++j) {
			vm->functions[i].constants[j].name = NULL; //delta_read_string(f);
			
			DeltaVariableType type = delta_read_int(f);
			vm->functions[i].constants[j].type = type;
			if(type == DELTA_TYPE_NULL) {
				// do nothing
			} else if(type == DELTA_TYPE_NUMBER) {
				vm->functions[i].constants[j].value.number = delta_read_double(f);
			} else if(type == DELTA_TYPE_STRING) {
				// TODO: not binary safe
				vm->functions[i].constants[j].value.ptr = delta_read_string(f);
			}
			
			vm->functions[i].constants[j].ram_location = delta_read_int(f);
			vm->functions[i].constants[j].size = delta_read_int(f);
		}
	}
	
	// clean up
	fclose(f);
	return vm;
}

/*
 *  Delta programming language
 */

#include "DeltaCompiler.h"
#include "delta/macros.h"
#include "delta/structs/DeltaClass.h"


struct DeltaCompiler* new_DeltaCompiler(int total_objects)
{
	struct DeltaCompiler *c = (struct DeltaCompiler*) malloc(sizeof(struct DeltaCompiler));
	int i;
	
	c->alloc_functions = DELTA_MAX_FUNCTIONS;
	c->total_functions = 0;
	c->functions = (struct DeltaCompiledFunction*)
		calloc(c->alloc_functions, sizeof(struct DeltaCompiledFunction));
	
	c->alloc_classes = 100;
	c->total_classes = 0;
	c->classes = (struct DeltaClass**)
		calloc(c->alloc_classes, sizeof(struct DeltaClass*));
	for(i = 0; i < c->alloc_classes; ++i)
		c->classes[i] = new_DeltaClass();
	
	for(i = 0; i < c->alloc_functions; ++i) {
		c->functions[i].alloc_ins = 100;
		c->functions[i].total_ins = 0;
		c->functions[i].ins = (struct DeltaInstruction*)
			calloc(c->functions[i].alloc_ins, sizeof(struct DeltaInstruction));
		
		c->functions[i].alloc_vars = 100;
		c->functions[i].total_vars = 0;
		c->functions[i].vars = (struct DeltaVariable*)
			calloc(c->functions[i].alloc_vars, sizeof(struct DeltaVariable));
		
		c->functions[i].alloc_labels = 100;
		c->functions[i].total_labels = 0;
		c->functions[i].labels = (struct DeltaLabel*)
			calloc(c->functions[i].alloc_labels, sizeof(struct DeltaLabel));
		
		c->functions[i].alloc_constants = 100;
		c->functions[i].total_constants = 0;
		c->functions[i].constants = (struct DeltaVariable*)
			calloc(c->functions[i].alloc_constants, sizeof(struct DeltaVariable));
	}
	
	return c;
}

void free_DeltaCompiler(struct DeltaCompiler *c)
{
	int i;
	for(i = 0; i < c->alloc_functions; ++i) {
		free(c->functions[i].ins);
		free(c->functions[i].vars);
		free(c->functions[i].labels);
		free(c->functions[i].constants);
	}
	
	free(c->functions);
	free(c);
}

/*
 *  Delta programming language
 */

#include "DeltaCompiler.h"
#include "delta/macros.h"


struct DeltaCompiler* new_DeltaCompiler(int total_objects)
{
	struct DeltaCompiler *c = (struct DeltaCompiler*) malloc(sizeof(struct DeltaCompiler));
	
	c->alloc_functions = DELTA_MAX_FUNCTIONS;
	c->total_functions = 0;
	c->functions = (struct DeltaCompiledFunction*)
		calloc(c->alloc_functions, sizeof(struct DeltaCompiledFunction));
	
	int i;
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

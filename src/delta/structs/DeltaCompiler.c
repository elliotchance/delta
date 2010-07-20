/*
 *  Delta programming language
 */

#include "DeltaCompiler.h"


struct DeltaCompiler* new_DeltaCompiler(int total_objects)
{
	struct DeltaCompiler *c = (struct DeltaCompiler*) malloc(sizeof(struct DeltaCompiler));
	
	c->alloc_ins = 100;
	c->total_ins = 0;
	c->ins = (struct DeltaInstruction*) malloc(c->alloc_ins * sizeof(struct DeltaInstruction));
	
	c->alloc_vars = 100;
	c->total_vars = 0;
	c->vars = (struct DeltaVariable*) malloc(c->alloc_vars * sizeof(struct DeltaVariable));
	
	c->alloc_labels = 100;
	c->total_labels = 0;
	c->labels = (struct DeltaLabel*) malloc(c->alloc_labels * sizeof(struct DeltaLabel));
	
	c->alloc_constants = 100;
	c->total_constants = 0;
	c->constants = (struct DeltaVariable*)
		malloc(c->alloc_constants * sizeof(struct DeltaVariable));
	
	return c;
}

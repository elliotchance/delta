/*
 *  Delta programming language
 */

#ifndef _DELTA_INS_H
#define _DELTA_INS_H


#define ins(name) void ins_##name(DI *d)
#define delta_function(name) void delta_function_##name(DI *d)
#define func(name) delta_function_##name

#include "structs.h"
#include "vm.h"
#include "cast.h"
#include "functions.h"


#define DELTA_ARGS        ((d->args - 1) / 2)
#define DELTA_DEST        d->arg[0]

#define DELTA_ARG0        d->arg[2]
#define DELTA_ARG1        d->arg[4]
#define DELTA_ARG2        d->arg[6]
#define DELTA_ARG3        d->arg[8]
#define DELTA_ARG4        d->arg[10]
#define DELTA_ARG5        d->arg[12]
#define DELTA_ARG6        d->arg[14]
#define DELTA_ARG7        d->arg[16]

#define DELTA_ARG0_NAME   d->arg[1]
#define DELTA_ARG1_NAME   d->arg[3]
#define DELTA_ARG2_NAME   d->arg[5]
#define DELTA_ARG3_NAME   d->arg[7]
#define DELTA_ARG4_NAME   d->arg[9]
#define DELTA_ARG5_NAME   d->arg[11]
#define DELTA_ARG6_NAME   d->arg[13]
#define DELTA_ARG7_NAME   d->arg[15]

#define DELTA_ARG(i)      d->arg[(i * 2) + 2]
#define DELTA_ARG_NAME(i) d->arg[(i * 2) + 1]

#define DELTA_PRINT_INS_ARGS(__BYTECODE) \
	printf("*BYTECODE_%s (", #__BYTECODE); \
	int i; \
	for(i = 0; i < DELTA_ARGS + 1; ++i) \
	printf(" %d", d->arg[i]); \
	printf(" )\n");

#define DELTA_RETURN_NULL \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_NULL; \
	return;\
}

#define DELTA_RETURN_NAN \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_NUMBER; \
	ram[DELTA_DEST]->value.number = NAN; \
	return;\
}

#define DELTA_RETURN_BOOLEAN(return_value) \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_BOOLEAN; \
	ram[DELTA_DEST]->value.number = (return_value == 0 ? 0 : 1); \
	return; \
}

#define DELTA_RETURN_NUMBER(return_value) \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_NUMBER; \
	ram[DELTA_DEST]->value.number = return_value; \
	return; \
}

#define DELTA_RETURN_ZERO \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_NUMBER; \
	ram[DELTA_DEST]->value.number = 0; \
	return; \
}

#define DELTA_RETURN_STRING(return_value) \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_STRING; \
	ram[DELTA_DEST]->value.ptr = return_value; \
	return; \
}

#define DELTA_RETURN_RESOURCE(return_value, resource_id) \
{ \
	ram[DELTA_DEST]->type = DELTA_TYPE_RESOURCE; \
	ram[DELTA_DEST]->value.resource.ptr = (char*) return_value; \
	ram[DELTA_DEST]->value.resource.id = resource_id; \
	return; \
}


ins(ADD); // +
ins(AG1); // Get first dimention array
ins(AS1); // Assign first dimention array
ins(CEQ); // ==
ins(CGE); // >=
ins(CGT); // >
ins(CLE); // <=
ins(CLT); // <
ins(CNE); // !=
ins(DEC); // --
ins(DIV); // /
ins(GTO); // goto label
ins(INC); // ++
ins(LBL); // begin label
ins(MUL); // *
ins(NUL); // no action
ins(RTN); // return
ins(SET); // =
ins(SUB); // -


#endif

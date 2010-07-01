/*
 *  ins.h
 *  jit
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#ifndef _DELTA_INS_H
#define _DELTA_INS_H

#include "structs.h"
#include "vm.h"
#include "cast.h"


#define ins(name) void ins_##name(DI *d)

#define DELTA_ARGS   (d->args - 1)
#define DELTA_DEST   d->arg[0]
#define DELTA_ARG0   d->arg[1]
#define DELTA_ARG1   d->arg[2]
#define DELTA_ARG2   d->arg[3]
#define DELTA_ARG3   d->arg[4]
#define DELTA_ARG4   d->arg[5]
#define DELTA_ARG5   d->arg[6]
#define DELTA_ARG6   d->arg[7]
#define DELTA_ARG7   d->arg[8]
#define DELTA_ARG(i) d->arg[i + 1]

#define DELTA_PRINT_INS_ARGS(__BYTECODE) \
printf("*BYTECODE_%s (", #__BYTECODE); \
int i; \
for(i = 0; i < DELTA_ARGS + 1; ++i) \
printf(" %d", d->arg[i]); \
printf(" )\n");

#define DELTA_RETURN_NULL \
	ram[DELTA_DEST]->type = DELTA_TYPE_NULL; \
	return;

#define DELTA_RETURN_NUMBER(return_value) \
	ram[DELTA_DEST]->type = DELTA_TYPE_NUMBER; \
	ram[DELTA_DEST]->value.number = return_value; \
	return;

double delta_cast_number(int address);

ins(ADD); // +
ins(APH); // array_push()
ins(CEQ); // ==
ins(CGE); // >=
ins(CGT); // >
ins(CLE); // <=
ins(CLT); // <
ins(CNE); // !=
ins(COS); // cos()
ins(DEC); // --
ins(DIV); // /
ins(GTO); // goto label
ins(INC); // ++
ins(LBL); // begin label
ins(MUL); // *
ins(NUL); // no action
ins(OUT); // print()
ins(OUL); // println()
ins(RTN); // return
ins(SET); // =
ins(SIN); // sin()
ins(SLN); // strlen()
ins(SQT); // sqrt()
ins(SST); // substr()
ins(SUB); // -
ins(TAN); // tan()


#endif

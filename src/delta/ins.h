/*
 *  ins.h
 *  jit
 *
 *  Created by Elliot Chance on 25/06/10.
 */

#ifndef _DELTA_INS_H
#define _DELTA_INS_H

#include "structs.h"


#define ins(name) void ins_##name(DI *d)

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
ins(RTN); // return
ins(SET); // =
ins(SIN); // sin()
ins(SQT); // sqrt()
ins(SUB); // -
ins(TAN); // tan()


#endif

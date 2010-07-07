/*
 *  Delta programming language
 */

#ifndef _DELTA_INS_H
#define _DELTA_INS_H


#include "../macros.h"


DELTA_INS(ADD); // +
DELTA_INS(AG1); // Get first dimention array
DELTA_INS(AS1); // Assign first dimention array
DELTA_INS(CEQ); // ==
DELTA_INS(CGE); // >=
DELTA_INS(CGT); // >
DELTA_INS(CLE); // <=
DELTA_INS(CLT); // <
DELTA_INS(CNE); // !=
DELTA_INS(DEC); // --
DELTA_INS(DIV); // /
DELTA_INS(GTO); // goto label
DELTA_INS(INC); // ++
DELTA_INS(LBL); // begin label
DELTA_INS(MUL); // *
DELTA_INS(NUL); // no action
DELTA_INS(RTN); // return
DELTA_INS(SET); // =
DELTA_INS(SUB); // -


#endif

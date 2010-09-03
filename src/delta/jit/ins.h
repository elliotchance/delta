/*
 *  Delta programming language
 */

#ifndef _DELTA_INS_H
#define _DELTA_INS_H


#include "../macros.h"


DELTA_INS(ADD); // +
DELTA_INS(AG1); // Get first dimention array
DELTA_INS(AND); // Boolean AND
DELTA_INS(ARG); // copy a function arg to a local location
DELTA_INS(AS1); // Assign first dimention array
DELTA_INS(CAL); // Call a function
DELTA_INS(CEQ); // ==
DELTA_INS(CGE); // >=
DELTA_INS(CGT); // >
DELTA_INS(CLE); // <=
DELTA_INS(CLT); // <
DELTA_INS(CNE); // !=
DELTA_INS(DEC); // --
DELTA_INS(DIV); // /
DELTA_INS(INC); // ++
DELTA_INS(LBL); // begin label
DELTA_INS(MOD); // %
DELTA_INS(MUL); // *
DELTA_INS(NAD); // fastmath +
DELTA_INS(NDE); // fastmath --
DELTA_INS(NDV); // fastmath /
DELTA_INS(NIN); // fastmath ++
DELTA_INS(NOT); // Boolean NOT
DELTA_INS(NUL); // no action
DELTA_INS(NMD); // fastmath %
DELTA_INS(NMU); // fastmath *
DELTA_INS(NSB); // fastmath -
DELTA_INS(ORR); // Boolean OR
DELTA_INS(RTN); // return
DELTA_INS(SAP); // .
DELTA_INS(SET); // =
DELTA_INS(SUB); // -
DELTA_INS(NEQ); // fastmath ==
DELTA_INS(NGE); // fastmath >=
DELTA_INS(NGT); // fastmath >
DELTA_INS(NLE); // fastmath <=
DELTA_INS(NLT); // fastmath <
DELTA_INS(NNE); // fastmath !=
DELTA_INS(ZAR); // cast to ARRAY
DELTA_INS(ZBO); // cast to BOOLEAN
DELTA_INS(ZNL); // cast to NULL
DELTA_INS(ZNU); // cast to NUMBER
DELTA_INS(ZOB); // cast to OBJECT
DELTA_INS(ZST); // cast to STRING
DELTA_INS(ZRS); // cast to RESOURCE


#endif

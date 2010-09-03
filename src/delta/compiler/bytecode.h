/*
 *  Delta programming language
 */

#ifndef _DELTA_BYTECODE_H
#define _DELTA_BYTECODE_H


#define BYTECODE_NUL 0x00 // do nothing


//-----------------------------------------------
// operators
//-----------------------------------------------

#define BYTECODE_SET 0x10 // =
#define BYTECODE_ZNL 0x11 // cast to NULL
#define BYTECODE_ZBO 0x12 // cast to BOOLEAN
#define BYTECODE_ZNU 0x13 // cast to NUMBER
#define BYTECODE_ZST 0x14 // cast to STRING
#define BYTECODE_ZAR 0x15 // cast to ARRAY
#define BYTECODE_ZRS 0x16 // cast to RESOURCE
#define BYTECODE_ZOB 0x17 // cast to OBJECT

#define BYTECODE_ADD 0x20 // +
#define BYTECODE_SUB 0x21 // -
#define BYTECODE_MUL 0x22 // *
#define BYTECODE_DIV 0x23 // /
#define BYTECODE_MOD 0x24 // %
#define BYTECODE_INC 0x25 // ++
#define BYTECODE_DEC 0x26 // --
#define BYTECODE_AND 0x27 // &&
#define BYTECODE_ORR 0x28 // ||
#define BYTECODE_NOT 0x29 // !
#define BYTECODE_SAP 0x2A // .

#define BYTECODE_CEQ 0x30 // ==
#define BYTECODE_CNE 0x31 // !=
#define BYTECODE_CLT 0x32 // <
#define BYTECODE_CGT 0x33 // >
#define BYTECODE_CLE 0x34 // <=
#define BYTECODE_CGE 0x35 // >=

#define BYTECODE_AS1 0x40 // Set first dimension array

#define BYTECODE_AG1 0x50 // Get first dimension array

#define BYTECODE_NAD 0x60 // FastMath +
#define BYTECODE_NSB 0x61 // FastMath -
#define BYTECODE_NMU 0x62 // FastMath *
#define BYTECODE_NDV 0x63 // FastMath /
#define BYTECODE_NMD 0x64 // FastMath %
#define BYTECODE_NIN 0x65 // FastMath ++
#define BYTECODE_NDE 0x66 // FastMath --
#define BYTECODE_NND 0x67 // FastMath &&
#define BYTECODE_NOR 0x68 // FastMath ||
#define BYTECODE_NPW 0x69 // FastMath **

#define BYTECODE_NEQ 0x70 // FastMath ==
#define BYTECODE_NNE 0x71 // FastMath !=
#define BYTECODE_NLT 0x72 // FastMath <
#define BYTECODE_NGT 0x73 // FastMath >
#define BYTECODE_NLE 0x74 // FastMath <=
#define BYTECODE_NGE 0x75 // FastMath >=


//-----------------------------------------------
// statements, functions and blocks
//-----------------------------------------------

#define BYTECODE_LBL 0x80 // begin label
#define BYTECODE_GTO 0x81 // goto label
#define BYTECODE_IFS 0x82 // if
#define BYTECODE_PAT 0x83 // patch forward jump
#define BYTECODE_JMP 0x84 // jump forward
#define BYTECODE_CAL 0x85 // call function, linked at JIT compile time
#define BYTECODE_LOP 0x86 // loop

#define BYTECODE_RTN 0x90 // return
#define BYTECODE_ARG 0x91 // copy a function argument to a local location


#endif

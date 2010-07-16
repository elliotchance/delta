/*
 *  Delta programming language
 */

#ifndef _DELTA_BYTECODE_H
#define _DELTA_BYTECODE_H


#define BYTECODE_NUL 0x0000 // do nothing


//-----------------------------------------------
// operators
//-----------------------------------------------

#define BYTECODE_SET 0x0010 // =
// TODO: docme
#define BYTECODE_SAP 0x0011 // .

#define BYTECODE_ADD 0x0020 // +
#define BYTECODE_SUB 0x0021 // -
#define BYTECODE_MUL 0x0022 // *
#define BYTECODE_DIV 0x0023 // /
#define BYTECODE_MOD 0x0024 // %
#define BYTECODE_INC 0x0025 // ++
#define BYTECODE_DEC 0x0026 // --
// TODO: docme
#define BYTECODE_AND 0x0027 // &&
// TODO: docme
#define BYTECODE_ORR 0x0028 // ||
// TODO: docme
#define BYTECODE_NOT 0x0029 // !

#define BYTECODE_CEQ 0x0030 // ==
#define BYTECODE_CNE 0x0031 // !=
#define BYTECODE_CLT 0x0032 // <
#define BYTECODE_CGT 0x0033 // >
#define BYTECODE_CLE 0x0034 // <=
#define BYTECODE_CGE 0x0035 // >=

#define BYTECODE_AS1 0x0040 // Set first dimension array

#define BYTECODE_AG1 0x0050 // Get first dimension array

#define BYTECODE_NAD 0x0060 // FastMath +
#define BYTECODE_NSB 0x0061 // FastMath -
#define BYTECODE_NMU 0x0062 // FastMath *
#define BYTECODE_NDV 0x0063 // FastMath /
#define BYTECODE_NMD 0x0064 // FastMath %
#define BYTECODE_NIN 0x0065 // FastMath ++
#define BYTECODE_NDE 0x0066 // FastMath --

#define BYTECODE_NEQ 0x0070 // FastMath ==
#define BYTECODE_NNE 0x0071 // FastMath !=
#define BYTECODE_NLT 0x0072 // FastMath <
#define BYTECODE_NGT 0x0073 // FastMath >
#define BYTECODE_NLE 0x0074 // FastMath <=
#define BYTECODE_NGE 0x0075 // FastMath >=


//-----------------------------------------------
// statements, functions and blocks
//-----------------------------------------------

#define BYTECODE_LBL 0x0110 // begin label
#define BYTECODE_GTO 0x0111 // goto label
#define BYTECODE_IFS 0x0112 // if
#define BYTECODE_PAT 0x0113 // patch forward jump
#define BYTECODE_JMP 0x0114 // jump forward
#define BYTECODE_CAL 0x0115 // call function, linked at JIT compile time
#define BYTECODE_LOP 0x0116 // loop

#define BYTECODE_RTN 0x0120 // return


#endif

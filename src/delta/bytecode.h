/*
 *  Delta programming language
 */

#ifndef _DELTA_BYTECODE_H
#define _DELTA_BYTECODE_H

#define BYTECODE_NUL 0x0000 // do nothing


//-----------------------------------------------
// operators
//-----------------------------------------------

#define BYTECODE_SET 0x0010 /* =  */

#define BYTECODE_ADD 0x0020 /* +  */
#define BYTECODE_SUB 0x0021 /* -  */
#define BYTECODE_MUL 0x0022 /* *  */
#define BYTECODE_DIV 0x0023 /* /  */

#define BYTECODE_CEQ 0x0030 /* == */
#define BYTECODE_CNE 0x0031 /* != */
#define BYTECODE_CLT 0x0032 /* <  */
#define BYTECODE_CGT 0x0033 /* >  */
#define BYTECODE_CLE 0x0034 /* <= */
#define BYTECODE_CGE 0x0035 /* >= */

#define BYTECODE_INC 0x0040 /* ++ */
#define BYTECODE_DEC 0x0041 /* -- */


//-----------------------------------------------
// statements, functions and blocks
//-----------------------------------------------

#define BYTECODE_LBL 0x0110 // begin label
#define BYTECODE_GTO 0x0111 // goto label
#define BYTECODE_IFS 0x0112 // if
#define BYTECODE_PAT 0x0113 // patch forward jump
#define BYTECODE_JMP 0x0114 // jump forward
#define BYTECODE_CAL 0x0115 // call function, linked at JIT compile time

#define BYTECODE_RTN 0x0120 // return


#endif

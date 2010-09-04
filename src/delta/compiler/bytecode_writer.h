/*
 *  Delta programming language
 */

#include "structs.h"


void delta_bytecode_writer_line(struct DeltaCompiler *c, DeltaByteCode bc, char *bc_name,
							    int line_num, char *msg, struct DeltaInstruction ins,
							    int function_id);
void delta_bytecode_writer_init(struct DeltaCompiler *c);
void delta_bytecode_writer_function(struct DeltaCompiler *c, char *function_name);
void delta_bytecode_writer_end_function(struct DeltaCompiler *c, char *function_name);
void delta_bytecode_writer_class(struct DeltaCompiler *c, char *the_class_name);
void delta_bytecode_writer_end_class(struct DeltaCompiler *c, char *the_class_name);
void delta_bytecode_writer_indent(struct DeltaCompiler *c, int indent);
void delta_bytecode_writer_address(struct DeltaCompiler *c, int function_id, int i,
								   struct DeltaInstruction ins);

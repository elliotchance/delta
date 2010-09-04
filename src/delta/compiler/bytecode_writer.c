/*
 *  Delta programming language
 */

#include "bytecode_writer.h"
#include "delta/macros.h"
#include "delta/compiler/bytecode.h"
#include <sys/time.h>


static FILE *bytecode_writer = NULL;
static int bytecode_writer_indent = -1;
void delta_write_bytecode_init(struct DeltaCompiler *c)
{
	if(bytecode_writer == NULL) {
		bytecode_writer = fopen("test.bytecode.txt", "w");
		fprintf(bytecode_writer, "# Bytecode output generated %d\n\n", (int) time(NULL));
	}
}


void delta_write_bytecode_indent(struct DeltaCompiler *c, int indent)
{
	delta_write_bytecode_init(c);
	int i;
	for(i = 0; i < indent; ++i)
		fprintf(bytecode_writer, "  ");
	fflush(bytecode_writer);
}


// replace addresses with constants or variable names if possible
void delta_bytecode_writer_address(struct DeltaCompiler *c, int function_id, int i,
								   struct DeltaInstruction ins)
{
	if(ins.arg[i] == -1) {
		fprintf(bytecode_writer, "RETURN_BUFFER");
		return;
	}
	
	int j;
	for(j = 0; j < c->functions[function_id].total_constants; ++j) {
		if(c->functions[function_id].constants[j].ram_location == ins.arg[i]) {
			struct DeltaVariable constant = c->functions[function_id].constants[j];
			if(constant.type == DELTA_TYPE_NULL)
				fprintf(bytecode_writer, "NULL");
			else if(constant.type == DELTA_TYPE_NUMBER)
				fprintf(bytecode_writer, "%g", constant.value.number);
			else if(constant.type == DELTA_TYPE_STRING)
				fprintf(bytecode_writer, "\"%s\"", constant.value.ptr);
			else
				fprintf(bytecode_writer, "(error)");
			return;
		}
	}
	
	for(j = 0; j < c->functions[function_id].total_vars; ++j) {
		if(c->functions[function_id].vars[j].ram_location == ins.arg[i]) {
			fprintf(bytecode_writer, "~%s", c->functions[function_id].vars[j].name);
			return;
		}
	}

	fprintf(bytecode_writer, "#%d", ins.arg[i]);
}


void delta_write_bytecode_line(struct DeltaCompiler *c, DeltaByteCode bc, char *bc_name,
							   int line_num, char *msg, struct DeltaInstruction ins,
							   int function_id)
{
	delta_write_bytecode_init(c);
	delta_write_bytecode_indent(c, bytecode_writer_indent + 1);
	if(line_num > 0) {
		if(strlen(msg) == 0)
			fprintf(bytecode_writer, "%s (0x%x) # Line %d\n", bc_name, (int) bc, line_num);
		else
			fprintf(bytecode_writer, "%s (0x%x) # Line %d: %s\n", bc_name, (int) bc, line_num, msg);
	}
	else
		fprintf(bytecode_writer, "%s (0x%x) # %s\n", bc_name, (int) bc, msg);
	
	// arguments
	if(ins.args > 0) {
		delta_write_bytecode_indent(c, bytecode_writer_indent + 2);
		fprintf(bytecode_writer, "(");
		int i;
		for(i = 1; i < ins.args; ++i) {
			if(i > 1) {
				if(i % 2)
					fprintf(bytecode_writer, ", ");
				else {
					if(ins.bc == BYTECODE_CAL)
						fprintf(bytecode_writer, " => ");
					else
						fprintf(bytecode_writer, ", ");
				}
			}
			
			delta_bytecode_writer_address(c, function_id, i, ins);
		}
		fprintf(bytecode_writer, ") => ");
		delta_bytecode_writer_address(c, function_id, 0, ins);
		fprintf(bytecode_writer, "\n");
	}
	
	fflush(bytecode_writer);
}


void delta_write_bytecode_function(struct DeltaCompiler *c, char *function_name)
{
	++bytecode_writer_indent;
	delta_write_bytecode_init(c);
	delta_write_bytecode_indent(c, bytecode_writer_indent);
	fprintf(bytecode_writer, "- Function %s\n", function_name);
	fflush(bytecode_writer);
}


void delta_write_bytecode_end_function(struct DeltaCompiler *c, char *function_name)
{
	delta_write_bytecode_init(c);
	delta_write_bytecode_indent(c, bytecode_writer_indent);
	fprintf(bytecode_writer, "- End Function %s\n", function_name);
	fflush(bytecode_writer);
	--bytecode_writer_indent;
}


void delta_write_bytecode_class(struct DeltaCompiler *c, char *the_class_name)
{
	++bytecode_writer_indent;
	delta_write_bytecode_init(c);
	delta_write_bytecode_indent(c, bytecode_writer_indent);
	fprintf(bytecode_writer, "- Class %s\n", the_class_name);
	fflush(bytecode_writer);
}


void delta_write_bytecode_end_class(struct DeltaCompiler *c, char *the_class_name)
{
	delta_write_bytecode_init(c);
	delta_write_bytecode_indent(c, bytecode_writer_indent);
	fprintf(bytecode_writer, "- End Class %s\n", the_class_name);
	fflush(bytecode_writer);
	--bytecode_writer_indent;
}

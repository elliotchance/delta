/*
 *  Delta programming language
 */


#include "../macros.h"
#include "../structs.h"


char*           delta_read_string(FILE *fp);
int             delta_read_int(FILE *fp);
DeltaByteCode   delta_read_bytecode(FILE *fp);
struct DeltaVM* delta_load_file(const char* in_file);
double          delta_read_double(FILE *fp);
char            delta_read_char(FILE *fp);

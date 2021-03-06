/*
 *  Delta programming language
 */


#ifndef _DELTA_COMPILER_STRINGS_H
#define _DELTA_COMPILER_STRINGS_H 1


int    delta_is_number(char *word);
int    delta_is_string(char* test);
char*  delta_copy_string(char* str);
char*  delta_copy_substring(char* str, int start, int length);
char*  delta_extract_argument_key(char *arg);
int    delta_strpos(char *haystack, char *needle);
int    delta_strrchr(char *haystack, char needle);
void   delta_escape_string(char *in, int length);
int    stricmp(const char *s1, const char *s2);
char** delta_split(char *haystack, char *needle, int* elements);
char*  delta_trim(char *str);
int    delta_is_magic_string(char *str);
char*  delta_translate_magic_string(char *str);
char*  delta_int_to_string(int value);
int    delta_strpos_off(char *haystack, char *needle, int offset);
char*  delta_join(char* glue, char** items, int start, int len);
char*  delta_combine_paths(char* path1, char* path2);
char** delta_pop(char **array, int *size);
char** delta_push(char **array, int *size, char *element);
char*  delta_cwd();


#endif

/*
 *  Delta programming language
 */


#ifndef _DELTA_COMPILER_STRINGS_H
#define _DELTA_COMPILER_STRINGS_H 1


int   delta_is_number(char *word);
int   delta_is_string(char* test);
char* delta_copy_string(char* str);
char* delta_copy_substring(char* str, int start, int length);
char* delta_extract_argument_key(char *arg);
int   delta_strpos(char *haystack, char *needle);
int   delta_strrchr(char *haystack, char needle);


#endif

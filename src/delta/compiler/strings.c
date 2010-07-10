/*
 *  Delta programming language
 */

#include "strings.h"
#include "../macros.h"
#include <string.h>
#include <ctype.h>


int delta_is_number(char *word)
{
	int i, len = strlen(word);
	for(i = 0; i < len; ++i) {
		if(!isnumber(word[i]) && word[i] != '.' && word[i] != '-' && word[i] != '+' &&
		   word[i] != 'e')
			return 0;
	}
	return 1;
}


int delta_is_string(char* test)
{
	if(test == NULL)
		return DELTA_NO;
	if(test[0] == '"')
		return DELTA_YES;
	return DELTA_NO;
}


char* delta_copy_string(char* str)
{
	int len = strlen(str);
	char *r = (char*) malloc(len + 1);
	strcpy(r, str);
	return r;
}


char* delta_copy_substring(char* str, int at, int length)
{
	int len = length - at;
	char *r = (char*) malloc(len + 1);
	strncpy(r, str + at, len);
	return r;
}


char* delta_extract_argument_key(char *arg)
{
	// look for the operator
	int i, len = strlen(arg), found = -1;
	for(i = 0; i < len - 1; ++i) {
		if(arg[i] == '=' && arg[i + 1] == '>') {
			found = i;
			break;
		}
	}
	
	// a key was found
	if(found >= 0) {
		// chop the key off
		char *r = delta_copy_substring(arg, 0, found);
		
		// TODO: memory leak
		arg = delta_copy_substring(arg, found + 2, len);
		
		return r;
	}
	
	// no key was found
	return NULL;
}


/**
 * @brief Find the first occurrance.
 */
int delta_strpos(char *haystack, char *needle)
{
	char *p = strstr(haystack, needle);
	if (p)
		return p - haystack;
	return -1;
}


/**
 * @brief Find the last occurrance of character.
 */
int delta_strrchr(char *haystack, char needle)
{
	char *p = strrchr(haystack, needle);
	if (p)
		return p - haystack;
	return -1;
}


/**
 * @brief Escaping a string will convert escape sequences into their respective characters.
 * TODO: this does not yet support hexadecimal representation.
 */
void delta_escape_string(char *in, int length)
{
	/*
	 \a	Bell (beep)
	 \b	Backspace
	 \f	Formfeed
	 \n	Newline
	 \r	Return
	 \t	Tab
	 \\	Backslash
	 \'	Single quote
	 \"	Double quote
	 \xdd Hexadecimal representation
	 */
	
	int i, write = 0;
	for(i = 0; i < length; ++i, ++write) {
		if(in[write] == '\\') {
			if(in[write + 1] == 'a')
				in[write] = '\a';
			else if(in[write + 1] == 'b')
				in[write] = '\b';
			else if(in[write + 1] == 'f')
				in[write] = '\f';
			else if(in[write + 1] == 'n')
				in[write] = '\n';
			else if(in[write + 1] == 'r')
				in[write] = '\r';
			else if(in[write + 1] == 't')
				in[write] = '\t';
			else
				in[write] = in[write + 1];
			--write;
		}
	}
	
	// chop off the useless bits
	in[write] = 0;
}
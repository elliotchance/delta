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
		if(!isdigit(word[i]) && word[i] != '.' && word[i] != '-' && word[i] != '+' &&
		   word[i] != 'e')
			return 0;
	}
	return 1;
}


int delta_is_string(char* test)
{
	if(test == NULL)
		return DELTA_NO;
	if(test[0] == '"' || test[0] == '\'')
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


/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
int stricmp(const char *s1, const char *s2)
{
	while(toupper(*s1) == toupper(*s2)) {
		if(*s1 == 0)
			return 0;
		s1++;
		s2++;
	}
	
	return toupper(*(unsigned const char*) s1) - toupper(*(unsigned const char*) (s2));
}


char* delta_extract_argument_key(char *arg)
{
	// look for the operator
	int i, len = strlen(arg), found = -1;
	int b1 = 0; // ()
	int b2 = 0; // []
	int b3 = 0; // {}
	for(i = 0; i < len - 1; ++i) {
		if(arg[i] == '(')
			++b1;
		else if(arg[i] == ')')
			--b1;
		else if(arg[i] == '[')
			++b2;
		else if(arg[i] == ']')
			--b2;
		else if(arg[i] == '{')
			++b3;
		else if(arg[i] == '}')
			--b3;
		else if(arg[i] == '=' && arg[i + 1] == '>' && !b1 && !b2 && !b3) {
			found = i;
			break;
		}
	}
	
	// a key was found
	if(found >= 0)
		// chop the key off
		return delta_copy_substring(arg, 0, found);
	
	// no key was found
	return NULL;
}


/**
 * @brief Find the first occurrance.
 */
int delta_strpos(char *haystack, char *needle)
{
	char *p = strstr(haystack, needle);
	if(p)
		return p - haystack;
	return -1;
}


/**
 * @brief Find the first occurrance with an offset.
 */
int delta_strpos_off(char *haystack, char *needle, int offset)
{
	char *p = strstr(haystack + offset, needle);
	if(p)
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
			else if(in[write + 1] == 'x') {
				char x[3], *end;
				strncpy(x, in + write + 2, 2);
				in[write] = (char) strtol(x, &end, 16);
				i += 2;
			}
			else
				in[write] = in[write + 1];
			++i;
		}
		else in[write] = in[i];
	}
	
	// chop off the useless bits
	in[write] = 0;
}


char** delta_split(char *haystack, char *needle, int* elements)
{
	int offset = 0;
	
	// first count all the elements
	*elements = 0;
	while(1) {
		int pos = delta_strpos_off(haystack, needle, offset);
		if(pos < 0)
			break;
		
		offset += pos + strlen(needle);
		++*elements;
	}
	++*elements;
	
	char **r = (char**) calloc(*elements, sizeof(char*));
	offset = 0;
	int i;
	for(i = 0; 1; ++i) {
		int pos = delta_strpos_off(haystack, needle, offset);
		if(pos < 0)
			break;
		
		r[i] = (char*) malloc(pos - offset + 1);
		//if(i > 0)
		//	strncpy(r[i], haystack + offset, pos - strlen(needle));
		//else
			strncpy(r[i], haystack + offset, pos - offset);
		offset = pos + strlen(needle);
	}
	r[*elements - 1] = (char*) malloc(strlen(haystack) - offset + 1);
	strcpy(r[*elements - 1], haystack + offset);
	
	return r;
}


char* delta_trim(char *line)
{
	int left = 0, right = strlen(line) - 1, off = 0;
	
	// start
	for(; left < strlen(line); ++left)
		if(!isspace(line[left]))
			break;
	
	// end
	for(; right >= 0; --right, ++off)
		if(!isspace(line[right]))
			break;
	
	char *r = (char*) malloc(strlen(line) - left - off - left + 1);
	strncpy(r, line + left, strlen(line) - left - off);
	return r;
}


int delta_is_magic_string(char *str)
{
	int i, len = strlen(str);
	for(i = 0; i < len; ++i) {
		if(str[i] == '$')
			return 1;
	}
	
	// this is not a magic string
	return 0;
}


char* delta_translate_magic_string(char *str)
{
	char *r = (char*) malloc(64);
	bzero(r, 64);
	int i, j, len = strlen(str);
	
	for(i = 0, j = 0; i < len; ++i) {
		if(str[i] == '$') {
			strcpy(r + j, "\" . ");
			j += 4;
			
			// find the end of the expression
			++i;
			if(str[i] == '{') {
				++i;
				r[j++] = '(';
				for(; i < len; ++i) {
					if(str[i] == '}')
						break;
					r[j++] = str[i];
				}
				r[j++] = ')';
				++i;
			} else {
				for(; i < len; ++i) {
					if(!isalnum(str[i]))
						break;
					r[j++] = str[i];
				}
			}
			
			strcpy(r + j, " . \"");
			j += 4;
			r[j++] = str[i];
		}
		else
			r[j++] = str[i];
	}
	
	return r;
}


char* delta_int_to_string(int value)
{
	char *buf = (char*) malloc(32);
	sprintf(buf, "%d", value);
	return buf;
}

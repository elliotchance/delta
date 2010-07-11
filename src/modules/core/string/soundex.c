/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*
 * Public domain from Bob Jarvis
 */

char *soundex(char *instr, char *outstr)
{               /* ABCDEFGHIJKLMNOPQRSTUVWXYZ */
	char *table = "01230120022455012623010202";
	char *outptr = outstr;
	int count = 0;
	
	while(!isalpha(instr[0]) && instr[0])
		++instr;
	
	if(!instr[0])     /* Hey!  Where'd the string go? */
		return(NULL);
	
	if(toupper(instr[0]) == 'P' && toupper(instr[1]) == 'H')
	{
		instr[0] = 'F';
		instr[1] = 'A';
	}
	
	*outptr++ = (char)toupper(*instr++);
	
	while(*instr && count < 5)
	{
		if(isalpha(*instr) && *instr != *(instr-1))
		{
			*outptr = table[toupper(instr[0]) - 'A'];
			if(*outptr != '0')
			{
				++outptr;
				++count;
			}
		}
		++instr;
	}
	
	*outptr = '\0';
	return(outstr);
}


/**
 * @brief Calculate the soundex key of a string.
 * @syntax string soundex ( string str )
 *
 * Calculates the soundex key of str.
 * 
 * Soundex keys have the property that words pronounced similarly produce the same soundex key, and
 * can thus be used to simplify searches in databases where you know the pronunciation but not the
 * spelling. This soundex function returns a string 4 characters long, starting with a letter.
 *
 * This particular soundex function is one described by Donald Knuth in "The Art Of Computer
 * Programming, vol. 3: Sorting And Searching", Addison-Wesley (1973), pp. 391-392.
 *
 * @param str The input string.
 * @return Returns the soundex key as a string.
 */
DELTA_FUNCTION(soundex)
{
	int release_arg0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release_arg0);
	
	// perform soundex
	char *buf = (char*) malloc(32);
	soundex(arg0->value.ptr, buf);
	
	// be nice and release
	DELTA_RELEASE(release_arg0, arg0);
	DELTA_RETURN_STRING(buf);
}

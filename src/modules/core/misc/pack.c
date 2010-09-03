/*
 *  Delta programming language
 */

#include "delta/delta.h"
#include <machine/endian.h>


inline unsigned short endian_swap_u16(unsigned short x)
{
    return (x>>8) | (x<<8);
}


inline unsigned int endian_swap_u32(unsigned int x)
{
    return (x>>24) |
		((x<<8) & 0x00FF0000) |
		((x>>8) & 0x0000FF00) |
		(x<<24);
}


inline unsigned long long endian_swap_u64(unsigned long long x)
{
    return (x>>56) | 
		((x<<40) & 0x00FF000000000000) |
		((x<<24) & 0x0000FF0000000000) |
		((x<<8)  & 0x000000FF00000000) |
		((x>>8)  & 0x00000000FF000000) |
		((x>>24) & 0x0000000000FF0000) |
		((x>>40) & 0x000000000000FF00) |
		(x<<56);
}


/**
 * Supported	Code	Description
 * NO			a		NUL-padded string
 * NO			A		SPACE-padded string
 * NO			h		Hex string, low nibble first
 * NO			H		Hex string, high nibble first
 * NO			c		signed char
 * NO			C		unsigned char
 * NO			s		signed short (always 16 bit, machine byte order)
 * NO			S		unsigned short (always 16 bit, machine byte order)
 * NO			n		unsigned short (always 16 bit, big endian byte order)
 * NO			v		unsigned short (always 16 bit, little endian byte order)
 * NO			i		signed integer (machine dependent size and byte order)
 * NO			I		unsigned integer (machine dependent size and byte order)
 * NO			l		signed long (always 32 bit, machine byte order)
 * NO			L		unsigned long (always 32 bit, machine byte order)
 * NO			N		unsigned long (always 32 bit, big endian byte order)
 * NO			V		unsigned long (always 32 bit, little endian byte order)
 * NO			f		float (machine dependent size and representation)
 * NO			d		double (machine dependent size and representation)
 * NO			x		NUL byte
 * NO			X		Back up one byte
 * NO			@		NUL-fill to absolute position
 * NO			*		Repeat the last character for the rest of the arguments.
 */
DELTA_FUNCTION(pack)
{
	// prepare arguments
	int release, i, total_size = 0;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release);
	char *format = arg0->value.ptr;
	
	// first we need to count how much ram we will use
	for(i = 0; i < arg0->size; ++i) {
		switch(format[i]) {
			case 'a':
				DELTA_TRIGGER_ERROR("'a' is not supported, ignoring.", DELTA_ERROR_ERROR);
				
			case 'A':
				DELTA_TRIGGER_ERROR("'A' is not supported, ignoring.", DELTA_ERROR_ERROR);
				
			case 'h':
				DELTA_TRIGGER_ERROR("'h' is not supported, ignoring.", DELTA_ERROR_ERROR);
				
			case 'H':
				DELTA_TRIGGER_ERROR("'H' is not supported, ignoring.", DELTA_ERROR_ERROR);
				
			case 'X':
				DELTA_TRIGGER_ERROR("'X' is not supported, ignoring.", DELTA_ERROR_ERROR);
				
			case '@':
				DELTA_TRIGGER_ERROR("'@' is not supported, ignoring.", DELTA_ERROR_ERROR);
				
			case '*':
				DELTA_TRIGGER_ERROR("'*' is not supported, ignoring.", DELTA_ERROR_ERROR);
			
			case 'c':
			case 'C':
			case 'x':
				total_size += sizeof(char);
				break;
				
			case 's':
			case 'S':
			case 'n':
			case 'v':
				total_size += sizeof(short);
				break;
				
			case 'i':
			case 'I':
				total_size += sizeof(int);
				break;
				
			case 'l':
			case 'L':
			case 'N':
			case 'V':
				total_size += sizeof(long);
				break;
				
			case 'f':
				total_size += sizeof(float);
				break;
				
			case 'd':
				total_size += sizeof(double);
				break;
		}
	}
	
	// write binary string
	char *result = (char*) malloc(total_size);
	int reslen = 0, argid = 1;
	
	for(i = 0; i < arg0->size; ++i, ++argid) {
		if(format[i] == 'c') {
			char temp = (char) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(char));
			reslen += sizeof(char);
			continue;
		}
		
		if(format[i] == 'C') {
			unsigned char temp = (unsigned char) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(char));
			reslen += sizeof(char);
			continue;
		}
		
		if(format[i] == 'd') {
			double temp = delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(double));
			reslen += sizeof(double);
			continue;
		}
		
		if(format[i] == 'f') {
			float temp = (float) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(float));
			reslen += sizeof(float);
			continue;
		}
		
		if(format[i] == 'i') {
			int temp = (int) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(int));
			reslen += sizeof(int);
			continue;
		}
		
		if(format[i] == 'I') {
			unsigned int temp = (unsigned int) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(int));
			reslen += sizeof(int);
			continue;
		}
		
		if(format[i] == 'l') {
			long temp = (long) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(long));
			reslen += sizeof(long);
			continue;
		}
		
		if(format[i] == 'L') {
			unsigned long temp = (unsigned long) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(long));
			reslen += sizeof(long);
			continue;
		}
		
		if(format[i] == 'n') {
			DELTA_TRIGGER_ERROR("'n' will use machine-dependant endianness.", DELTA_ERROR_WARNING);
			
			unsigned short temp = (unsigned short) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(short));
			reslen += sizeof(short);
			continue;
		}
		
		if(format[i] == 'N') {
			DELTA_TRIGGER_ERROR("'N' will use machine-dependant endianness.", DELTA_ERROR_WARNING);
			
			unsigned long temp = (unsigned long) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(long));
			reslen += sizeof(long);
			continue;
		}
		
		if(format[i] == 'v') {
			DELTA_TRIGGER_ERROR("'v' will use machine-dependant endianness.", DELTA_ERROR_WARNING);
			
			unsigned short temp = (unsigned short) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(short));
			reslen += sizeof(short);
			continue;
		}
		
		if(format[i] == 'V') {
			DELTA_TRIGGER_ERROR("'V' will use machine-dependant endianness.", DELTA_ERROR_WARNING);
			
			unsigned long temp = (unsigned long) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(long));
			reslen += sizeof(long);
			continue;
		}
		
		if(format[i] == 's') {
			short temp = (short) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(short));
			reslen += sizeof(short);
			continue;
		}
		
		if(format[i] == 'S') {
			unsigned short temp = (unsigned short) delta_cast_number(DELTA_ARG(argid));
			memmove(result + reslen, (char*) &temp, sizeof(short));
			reslen += sizeof(short);
			continue;
		}
		
		if(format[i] == 'x') {
			memmove(result + reslen, '\0', sizeof(char));
			reslen += sizeof(char);
			continue;
		}
	}
	
	// free arguments and return
	DELTA_RELEASE(release, arg0);
	DELTA_RETURN_BINARY_STRING(result, reslen);
}

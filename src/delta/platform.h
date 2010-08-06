/*
 *  Delta programming language
 */

#ifndef _DELTA_PLATFORM_H
#define _DELTA_PLATFORM_H 1


//--------------------------------
// Operating System
//--------------------------------

#if defined(linux) || \
    defined(__linux)

	#define DELTA_PLATFORM_LINUX 1

#endif

#if defined(macintosh) || \
    defined(Macintosh) || \
	defined(__APPLE__) || \
    defined(__MACH__)

	#define DELTA_PLATFORM_MAC 1

#endif

#if defined(_WIN32) || \
    defined(_WIN64) || \
    defined(__WIN32__) || \
    defined(__TOS_WIN__) || \
	defined(__WINDOWS__)

	#define DELTA_PLATFORM_WINDOWS 1

#endif


//--------------------------------
// Endianness
//--------------------------------


#if DELTA_PLATFORM_LINUX
	#define DELTA_ENDIAN_UNKNOWN 1
#endif


#if DELTA_PLATFORM_MAC
	#define DELTA_ENDIAN_UNKNOWN 1
#endif


#if DELTA_PLATFORM_WINDOWS
	#define DELTA_ENDIAN_LITTLE 1
#endif


#endif

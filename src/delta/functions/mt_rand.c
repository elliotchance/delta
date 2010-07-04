/*
 *  Delta programming language
 */

#include "ins.h"
#include "../../modules/mt.h"
#include <sys/time.h>


/**
 * @brief Generate a better random value.
 * @syntax void mt_rand ( )
 * @syntax void mt_rand ( int min, int max )
 * 
 * Many random number generators of older libcs have dubious or unknown characteristics and are
 * slow. By default, PHP uses the libc random number generator with the rand() function.
 * The mt_rand() function is a drop-in replacement for this. It uses a random number generator with
 * known characteristics using the » Mersenne Twister, which will produce random numbers four times
 * faster than what the average libc rand() provides.
 *
 * If called without the optional min, max arguments mt_rand() returns a pseudo-random value between
 * 0 and mt_getrandmax(). If you want a random number between 5 and 15 (inclusive), for example, use
 * mt_rand(5, 15).
 *
 * @param min Optional lowest value to be returned (default: 0)
 * @param max Optional highest value to be returned (default: mt_getrandmax()).
 * @return A random integer value between min (or 0) and max (or mt_getrandmax(), inclusive).
 * @see mt_srand
 * @see mt_getrandmax
 * @see rand
 */
delta_function(mt_rand)
{
	unsigned long * b = mt_buffer;
    int idx = mt_index;
    unsigned long s;
    int i;
	
    if(idx == MT_LEN * sizeof(unsigned long)) {
        idx = 0;
        i = 0;
        for (; i < MT_IB; i++) {
            s = TWIST(b, i, i+1);
            b[i] = b[i + MT_IA] ^ (s >> 1) ^ MAGIC(s);
        }
        for (; i < MT_LEN-1; i++) {
            s = TWIST(b, i, i+1);
            b[i] = b[i - MT_IB] ^ (s >> 1) ^ MAGIC(s);
        }
        
        s = TWIST(b, MT_LEN-1, 0);
        b[MT_LEN-1] = b[MT_IA-1] ^ (s >> 1) ^ MAGIC(s);
    }
	
    mt_index = idx + sizeof(unsigned long);
	DELTA_RETURN_NUMBER(*(unsigned long *)((unsigned char *)b + idx));
}

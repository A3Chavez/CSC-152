/**
 * Write a C function that takes a buffer of memory as a parameter, treats the memory like
 * it's an array of uint32_t, reads each element, applies prof_swizzle to each (see next
 * Question for swizzle explanation), and writes the result back to the buffer. If the
 * number of bytes is not a multiple of 4, ignore the final 1, 2, or 3 bytes that are
 * beyond the last multiple of 4 (ie, only read and write the first nbytes/4 elements in
 * the buffer).
 *
 * Example 1: if the buffer has the following bytes (shown in hex): 01 02 03 04 05 06 07 08;
 * then your function should update the buffer to be: 02 01 04 03 06 05 08 07.
 *
 * Example 2: if the buffer has the following bytes (shown in hex): 01 02 03 04 05 06 07 08 09;
 * then your function should update the buffer to be: 02 01 04 03 06 05 08 07 09. In this example,
 * the 09 is ignored because it is beyond the last multiple of 4.
 *
 * The compiler compiles with -W -Wall -Werror settings, which means your code will not compile
 * if it generates any compiler errors or warnings. There is no partial credit on this problem.
 */
 
#include <stdint.h>

// returns the abcd->badc swizzle of x
uint32_t prof_swizzle(uint32_t x);

// Treats p like an array of uint32_t and updates the
// first nbytes/4 elements using prof_swizzle
void swizzle_buffer(void *p, int nbytes) {
    uint32_t *p32 = (uint32_t *)p;
    int iters = nbytes/4;
    for (int i=0; i<iters; i++) {
        p32[i] = prof_swizzle(p32[i]);
    }
}
 
/**
 * Write code that swaps each pair of bytes in a uint32_t. So, if 0x12345678 is passed in as a
 * parameter, 0x34127856 should be returned. Do this using bit shifting and bit masking techniques
 * similar to those seen in loading data big-endian on a little endian computer.
 *
 * The compiler compiles with -W -Wall -Werror settings, which means your code will not compile
 * if it generates any compiler errors or warnings. There is no partial credit on this problem.
 */
 
#include <stdint.h>

uint32_t swizzle(uint32_t x) {
    uint32_t a = (x & 0x000000FF) << 8;  // copy byte 0 and shift left 8 bits
    uint32_t b = (x & 0x0000FF00) >> 8;  // copy byte 1 and shift right 8 bits
    uint32_t c = (x & 0x00FF0000) << 8;  // copy byte 2 and shift left 8 bits
    uint32_t d = (x & 0xFF000000) >> 8;  // copy byte 3 and shift right 8 bits
    return a | b | c | d;                 // reassemble
}

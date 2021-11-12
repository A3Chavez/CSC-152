//----------------------------------Beginning of Question 1-----------------------------------------
/* In the reading, I give you C code for reading a uint32_t little endian. 
 * Modify it so that you have code that reads a uint32_t big endian.
 */
 
#include <stdint.h>

// Read the four bytes begining at p and assemble them big-endian
uint32_t load_uint32_big(void *p) {
    uint32_t *p32 = p;
    uint32_t x = p32[0];
  
    x = ((x>>24)&0xff) |        // move byte 3 to byte 0
      ((x<<8)&0xff0000) |     // move byte 1 to byte 2
      ((x>>8)&0xff00) |       // move byte 2 to byte 1
      ((x<<24)&0xff000000);   // byte 0 to byte 3
      
    return x;
}
//----------------------------------End of Question 1-----------------------------------------------

//----------------------------------Beginning of Question 2-----------------------------------------
/* In the reading, I give you C code for reading a uint32_t little endian. 
 * Modify it so that you have Java code that does the equivalent thing. Note 
 * that in Java, byte and int are guaranteed to be 8 and 32 bits, so there is 
 * no need to use alternative names like uint8_t and uint32_t.
 */
 
public class Le {

    // Read the four bytes begining at p and assemble them little-endian
    public static int load_int_little(byte[] p) {
        int i = ((0xFF & p[3]) << 24) | ((0xFF & p[2]) << 16) |
            ((0xFF & p[1]) << 8) | (0xFF & p[0]);
        return i;
    }
}
//----------------------------------End of Question 2-----------------------------------------------

//----------------------------------Beginning of Question 3-----------------------------------------
/* Write a C function that takes a buffer of memory as a parameter, treats the 
 * memory like it's an array of uint32_t, reads each element big-endian, adds them 
 * all together, and returns the sum. If the number of bytes is not a multiple of 4, 
 * ignore the final 1, 2, or 3 bytes that are beyond the last multiple of 4 (ie, only 
 * add together the first nbytes/4 elements in the buffer).
 *
 * Uint32_t is defined as an "unsigned integer" type and can hold any number 0 ... 232-1. 
 * So, when you read four bytes into this type of variable C treats it like any other number 
 * that can be added. Two uint32_t numbers added together yields a uint32_t result. Any overflow 
 * carry bit is discarded by the CPU. What this means is that mathematically, you are computing the 
 * sum "mod 232". (Do not do anything extra to accomplish this mod; it is "free" simply because that's 
 * how computer arithmetic works.)
 *
 * Example 1: if the buffer has the following bytes (shown in hex): 00 00 00 01 00 00 00 02;
 * then your function should return 3.
 *
 * Example 2: if the buffer has the following bytes (shown in hex): 00 00 00 01 FF FF FF FF 02;
 * then your function should return 0. 
 * In this example, the 02 is ignored because it is beyond the last multiple of 4.
 */
 
 #include <stdint.h>

// PASTE YOUR load_uint32_big function here
// Read the four bytes begining at p and assemble them big-endian
uint32_t load_uint32_big(void *p) {
  uint32_t *p32 = p;
  uint32_t x = p32[0];
  
  x = ((x>>24)&0xff) |        // move byte 3 to byte 0
      ((x<<8)&0xff0000) |     // move byte 1 to byte 2
      ((x>>8)&0xff00) |       // move byte 2 to byte 1
      ((x<<24)&0xff000000);   // byte 0 to byte 3
      
  return x;
}

// Treats p like an array of uint32_t and returns the sum of
// first nbytes/4 elements read big-endian
uint32_t sum32(void *p, int nbytes) {
    uint32_t result = 0;
    int max_nbytes = 0;
  
    if(nbytes % 4 == 0) {
      max_nbytes = nbytes;
    } 
    else {
      if(nbytes % 4 == 1)
        max_nbytes = nbytes - 1;
      else if(nbytes % 4 == 2)
        max_nbytes = nbytes - 2;
      else
        max_nbytes = nbytes - 3;
    }
    
    for(int i=0; i<max_nbytes; i+=4) {
      uint32_t *p32 = (uint32_t *)p;
      result += load_uint32_big((void *)p32+i);
    }
  
    return result;
}

//----------------------------------End of Question 3-----------------------------------------------

//----------------------------------Beginning of Question 4-----------------------------------------
/* Let each of the variables in the following pseudocode represent a uint32_t. The code scrambles 
 * a, b and c together in an invertible way.
 *
 * x = ROTL(a, 24)
 * y = ROTL(b, 9)
 * z = c
 * a = z xor y xor ((x and y) << 3)
 * b = y xor x xor ((x or z) << 1)
 * c = x xor (z << 1) xor ((y and z) << 2)
 *
 * This code has good "diffusion": changing any bit of a, b, or c causes multiple bits in 
 * widespread locations to change in the outcome. It also has good "confusion": the input-output
 * relation is complex. Invertibility and good confusion and diffusion are key buuilding blocks
 *  for a permutation in symmetric cryptography.
 *
 * Implement this pseudocode in C. Read what a, b, and c point at from memory; do the computation;
 * and write the results back to what a, b, and c point at. In this problem, do not worry about
 * endianness; just use C's natural uint32_t memory reads and writes. (Since this will be tested
 * on an Intel x86 CPU, this means that we are doing the reads and writes little-endian.)
 */

#include <stdint.h>

uint32_t rotl32(uint32_t x, int n) {
    return (x << n) | (x >> (32-n));
}

// Implement the pseudocode here
void scramble(uint32_t *a, uint32_t *b, uint32_t *c) {
  uint32_t x = rotl32(*a,24);
  uint32_t y = rotl32(*b,9);
  uint32_t z = *c;
  
  *a = (z ^ y) ^ ((x & y) << 3);
  *b = (y ^ x) ^ ((x | z) << 1);
  *c = x ^ (z << 1) ^ ((y & z) << 2);
}

//----------------------------------End of Question 4-----------------------------------------------

//----------------------------------Beginning of Question 5-----------------------------------------
/* During this class, we are going to use a cryptographic permutation as a building block for
 * several other algorithms. The previous problem had you write a function that thoroughly
 *  scrambles together three uint32_t values in an invertible way. In this problem you will use
 * the previous scrambling function repeatedly to scramble together a 48-byte buffer in an
 * invertible way. The following pseudocode assumes p32 holds the address of a 48-byte buffer and
 * is of type (uint32_t *). This treats the underlying buffer as if it were an array of uint32_t.
 * The expression p32+i evaluates to the address of the i-th element of this array.
 *
 * for round = 0 to 23
 *   scramble(p32+0, p32+4, p32+8)
 *   scramble(p32+1, p32+5, p32+9)
 *   scramble(p32+2, p32+6, p32+10)
 *   scramble(p32+3, p32+7, p32+11)
 *   if (round mod 4 == 0)   // ie, when round is 0, 4, 8, 12, 16, 20
 *     swap uint32_t values at p32+0 and p32+1
 *      swap uint32_t values at p32+2 and p32+3
 *      xor uint32_t value at p32+0 with (0x79379E00 xor round)
 *    if (round mod 4 == 2)   // ie, when round is 2, 6, 10, 14, 18, 22
 *      swap uint32_t values at p32+0 and p32+2
 *      swap uint32_t values at p32+1 and p32+3
 *
 * The end result is a permutation function with domain and codomain of all sequnces of 48 bytes.
 * The scrambling is thorough enough that the permutation resembles a randomly chosen one. Note,
 * though, that due to Kerckhoff's law we assume that every adversary knows this definition and
 * can compute it and its inverse. For this reason, I will call this a "fixed public random
 * permutation". It constantly amazes me that so little code can define an algorithm that will
 * prove to be so usefull.
 *
 * I will link my scramble code along with your submitted code and call it "prof_scramble".
 * This means that you can either paste your own scramble and use it, or not paste your scamble
 * and use mine.
 *
 * In this problem, do not worry about endianness; just use C's natural uint32_t memory reads
 * and writes. (Since this will be tested on an Intel x86 CPU, this means that we are doing the
 * reads and writes little-endian.)
 */

#include <stdint.h>

void swap(uint32_t *a, uint32_t *b) {
    uint32_t newb = *a;
    uint32_t newa = *b;
    *a = newa;
    *b = newb;
}

// You can call this instead of pasting your own scramble, either way is ok
void prof_scramble(uint32_t *a, uint32_t *b, uint32_t *c);

// Implement the permutation for the 48 bytes beginning at p
void perm384(void *p) {
  uint32_t *p32 = p;
  
  for(int rounds=0; rounds<24; rounds++) {
    prof_scramble((p32+0),(p32+4),(p32+8));
    prof_scramble((p32+1),(p32+5),(p32+9));
    prof_scramble((p32+2),(p32+6),(p32+10));
    prof_scramble((p32+3),(p32+7),(p32+11));
    
    if(rounds % 4 == 0) {
      swap((p32+0),(p32+1));
      swap((p32+2),(p32+3));
      *p32 = *p32 ^ (0x79379E00 ^ rounds);
    }
    if(rounds % 4 == 2) {
      swap((p32+0),(p32+2));
      swap((p32+1),(p32+3));
    }
  }
}
//----------------------------------End of Question 5-----------------------------------------------

//----------------------------------Beginning of Question 1----------------------------------------------------------
/* In class we saw how to turn a public random permutation into a block cipher. The suggested 
 * construction was E(k, x) = k xor perm384(k xor x). A 384-bit key is overkill, so let's define 
 * perm384_blockcipher(k, x) as requiring 256 bit k, 384 bit x, and defining it as 
 * (k || <0>128) xor perm384((k || <0>128) xor x) where || is concatenation and k || <0>128 is 128 bits of zero.
 *
 * Implement this block cipher. To simplify things, I will supply my perm384 at link time as prof_perm384. You 
 * should call this one instead of your own.
 *
 * NOTE: You do not need to worry about endianess. build your perm384 input in an appropriate array, call 
 * prof_perm384, then update the array by xoring appropriately.
 */
 
#include <stdint.h>

void prof_perm384(void *p);

// dst = (k || 0) xor perm384((k || 0) xor src)
// k is 32B, src is 48B and dst is 48B
void perm384_blockcipher(void *k, void *src, void *dst) {
    uint8_t buf[48];
    uint8_t *s8 = (uint8_t *)src;
    uint8_t *d8 = (uint8_t *)dst;
    uint8_t *k8 = (uint8_t *)k;
    for (int i=0; i<32; i++)
        buf[i] = s8[i] ^ k8[i];
    for (int i=32; i<48; i++)
        buf[i] = s8[i];
    prof_perm384(buf);
    for (int i=0; i<32; i++)
        d8[i] = buf[i] ^ k8[i];
    for (int i=32; i<48; i++)
        d8[i] = buf[i];
}

//----------------------------------End of Question 1----------------------------------------------------------------

//----------------------------------Beginning of Question 2----------------------------------------------------------
/* Implement CTR mode encryption using perm384_blockcipher. To simplify things, I will supply my perm384_blockcipher
 * at link time as prof_perm384_blockcipher. You should call this one instead of your own.
 *
 * Your CTR implementation should accept a 48 byte CTR block as input and use it as the input block to 
 * perm384_blockcipher. This means that a client of your CTR mode code would allocate their own 48 byte block, and
 * initialize the block with their desired nonce and initial counter value (this is how OpenSSL does CTR too). After
 * each call to perm384_blockcipher, you should increment the block by one so that it's ready for the next call to
 * perm384_blockcipher. Since it's conventional to increment the counter big-endian (ie, increment the 48th byte
 * and carry any carry bit to the left), you'll need to increment it with code similar to the following (which
 * assumes buf is a pointer to uint8_t).
 *
 * i = 47;  // last byte of 384 bit block
 * buf[i] += 1;
 * while (buf[i] == 0) {  // detect carry-out
 *   i -= 1;
 *   buf[i] += 1;
 * }
 */
 
#include <stdint.h>

// dst = (k || 0) xor perm384((k || 0) xor src)
// k is 32B, src is 48B and dst is 48B
void prof_perm384_blockcipher(void *k, void *src, void *dst);

void increment(void *p) {
    uint8_t *p8 = (uint8_t *)p + 47;
    *p8 += 1;              // incement byte at p8
    while (*p8 == 0) {     // if it wrapped to 0
        p8 -= 1;           // move pointer back 1 byte
        *p8 += 1;          // and increment that byte too
    }
}

int min(int a, int b) { if (b<a) a=b; return a; } 

// CTR mode encrypting or decrypting nbytes from src to dst. key and blk must
// be 32 and 48 bytes. blk is big-endian incremented ceiling(nbytes/48) times.
void perm384_blockcipher_ctr(void *key, void *blk, void *src, void *dst, int nbytes) {
    uint8_t *s8 = (uint8_t *)src;
    uint8_t *d8 = (uint8_t *)dst;
    uint8_t buf[48];
    while (nbytes > 0) {
        prof_perm384_blockcipher(key, blk, buf);
        increment(blk);
        int bytes_to_xor = min(nbytes, 48);
        for (int i=0; i<bytes_to_xor; i++)
            d8[i] = s8[i] ^ buf[i];
        s8 += bytes_to_xor;
        d8 += bytes_to_xor;
        nbytes -= bytes_to_xor;
    }
}

//----------------------------------End of Question 2----------------------------------------------------------------

//----------------------------------Beginning of Question 3----------------------------------------------------------
/* In this function, b has the address of a memory buffer that is num_bytes long. The function should repeatedly
 * copy the 16 byte pattern that pattern16 points at into the memory buffer until num_bytes have been written. If
 * num_bytes is not a multple of 16, the final wriite of the 16 byte pattern should be truncated to finish filling
 * the buffer.
 *
 * For example if the 16 bytes that pattern16 points at is 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff, then
 * memset(b, 20, pattern16) should write to the buffer pointed at by p the 20 bytes
 * 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00 11 22 33.
 *
 * Use SSE instructions to improve efficiency. Here's pseudocode.
 *
 * x = SSE unaligned load from pattern16
 * while (num_bytes >= 16)
 *   SSE unaligned store x to p
 *   advance p by 16 bytes
 *   decrement num_bytes by 16
 * while (num_bytes > 0)
 *   store 1 byte from pattern16 to p
 *   advance p by 1 byte
 *   advance pattern16 by 1 byte
 *   decrement num_bytes by 1
 *
 * NOTE: There are alternatives to this pseudocode. It is only for demonstartion purposes. I want you to practice
 * with your pointers, so do not use any library calls in your function.
 */
 
#include <stdint.h>
#include <immintrin.h>

// Note: pointer arithmetic on (void *) pointers is non-standard
// GCC and Clang support it, so I use it here. If p is a (void *) type
// then p = p + 1 increments p by one byte. The standard way of doing this
// would be p = (char *)p + 1.

// Beginning at p, copy the 16 bytes that pattern16 points at
// num_bytes / 16 times and then copy the first num_bytes % 16
// bytes from pattern16
void memset16(void *p, void *pattern16, int num_bytes) {
    __m128i x = _mm_loadu_si128((__m128i *)pattern16);
    while (num_bytes >= 16) {
        _mm_storeu_si128((__m128i *)p, x);
        p += 16;
        num_bytes -= 16;
    }
    while (num_bytes > 0) {
        *(uint8_t *)p = *(uint8_t *)pattern16;
        p += 1;
        pattern16 += 1;
        num_bytes -= 1;
    }
}

//----------------------------------End of Question 3----------------------------------------------------------------
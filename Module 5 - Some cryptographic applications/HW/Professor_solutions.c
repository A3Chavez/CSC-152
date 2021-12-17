/*****************************Question 1 Start***********************************/
/**
 * To set up an encryption we need to initialize the hash and checksum values. 
 * We saw in class that OCB uses in its tweakable block cipher the hash 
 * h(tweak) = (iv)(2)^tweak using Galois field math. This means the hash value
 * should be initialized as just h(0) = iv. Since the iv is supposed to be random
 * for each plaintext, we can approximate this as iv = E(nonce) where E is our
 * block cipher. For E, use perm384_blockcipher and key k.
 *
 * The checksum is an accumulator, so initializing it to zeros is the right
 * thing to do.
 *
 * Note: The nonce is 12 bytes and should be 10* padded into a 48 byte block
 * cipher input.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// dst = (k || 0) xor perm384((k || 0) xor src)
// k is 32B, src is 48B and dst is 48B
void prof_perm384_blockcipher(void *k, void *src, void *dst);

// Xor 48B from src1 and src2 and write result to dst
void xor_block(void *src1, void *src2, void *dst) {
    uint8_t *d = (uint8_t *)dst;
    uint8_t *s1 = (uint8_t *)src1;
    uint8_t *s2 = (uint8_t *)src2;
    for (int i=0; i<48; i++)
        d[i] = s1[i] ^ s2[i];
}

// Sets 48B checksum to zeros and 48B hash to E(nonce||10*)
// nonce is 12B, k is a 32B perm384_blockcipher key.
void ocb_setup(void *k, void *nonce, void *hash, void *checksum) {
    uint8_t tmp[48];
    // Set tmp to nonce || 10*, encipher with block cipher into hash
    memcpy(tmp, nonce, 12);
    memset(tmp+12, 0, 36);
    tmp[12] = 0x80;
    prof_perm384_blockcipher(k, tmp, hash);
    // Set checksum to zeros
    memset(checksum, 0, 48);
}

/*****************************Question 1 End************************************/
/*****************************Question 2 Start**********************************/
/**
 * The TBC used in OCB has a hash function that given one hash output, the next
 * hash output is easily generated. In the case of our 48 byte blockcipher,
 * pseudocode for generating the next hash output would look like this:
 *
 * next_hash(p):
 *   if highbit(p) == 1:
 *      p = (p << 1) xor 0x100D
 *   else
 *      p = p << 1
 *
 * This hash is treating p like it's a polynomial in GF(2^384) and multiplying it
 * by x. If this results in a degree 384 polynomial, the modular reduction is
 * handled by xor'ing x^384 + x^12 + x^3 + x^2 + x^0 (which is accomplished by
 * letting the x^384 term disappear during the shift and xor'ing 0x100D).
 *
 * Implement this psudocode below. We want the 48 bytes of memory whose
 * pointer is passed into the function to be effected as descibed. The
 * bitshift left should probably be handled in a loop, updating the first
 * byte, then the second byte, then the third, etc. If you want a challenge
 * the process may be done 64-bits at a time (which would be faster but
 * would require reading and writing the data into variables big-endian).
 * The choice is yours.
 *
 * Hint: If a and b are 8-bit variables and you want to shift a left one bit
 * position and have the leftmost bit of b become the new rightmost bit of a,
 * then you can do this as a = (a << 1 ) | (b >> 7).
 */

#include <stdint.h>

// Shift 48B pointed at by p left 1 bit. If overflow bit is 1,
// xor 0x100D into the last two bytes. Equivalent to p*x mod
// x^384 + x^12 + x^3 + x^2 + x^0 in GF(2^384)
void next_hash(void *p) {
    uint8_t *p8 = (uint8_t *)p;
    uint8_t highbit = p8[0] >> 7;
    for (int i=0; i<47; i++)
        p8[i] = (p8[i] << 1) | (p8[i+1] >> 7);
    p8[47] = p8[47] << 1;
    if (highbit) {
        p8[46] = p8[46] ^ 0x10;
        p8[47] = p8[47] ^ 0x0D;
    }
}

/*****************************Question 2 End************************************/
/*****************************Question 3 Start**********************************/
/**
 * We will write the encryption function in a way that allows streaming data to
 * be handled (ie, an incremental interface like we saw with the sponge hash).
 * The current hash and checksum values are passed into the encryption function,
 * and they are updated during the encryption process, ready for the next chunk
 * of encryption.
 *
 * You are given the current hash value and checksum. For each 48 byte chunk of
 * src you should: (i) update hash with next_hash, (ii) update checksum by
 * xor'ing into it the 48 byte chunk of src, and (iii) write the next 48 bytes
 * to dst as the result of hash xor E(hash xor the src chunk). For E, use
 * perm384_blockcipher and key k.
 *
 * You may assume that nbytes will always be a multiple of 48 
 * (ie, 0, 48, 96, etc). OCB is designed to handle data that is not a multiple
 * of 48, but we will not write that part.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// dst = (k || 0) xor perm384((k || 0) xor src)
// k is 32B, src is 48B and dst is 48B
void prof_perm384_blockcipher(void *k, void *src, void *dst);

// Shift 48B pointed at by p left 1 bit. If overflow bit is 1,
// xor 0x100D into the last two bytes. Equivalent to p*x mod
// x^384 + x^12 + x^3 + x^2 + x^0 in GF(2^384)
void prof_next_hash(void *p);

// Xor 48B from src1 and src2 and write result to dst
void xor_block(void *src1, void *src2, void *dst) {
    uint8_t *d = (uint8_t *)dst;
    uint8_t *s1 = (uint8_t *)src1;
    uint8_t *s2 = (uint8_t *)src2;
    for (int i=0; i<48; i++)
        d[i] = s1[i] ^ s2[i];
}

// OCB encrypt nbytes from src to dst. mask and checksum are preinitialized
// and should be updated before each block is processed. nbytes will be a
// multiple of 48 (ie, 0, 48, 96, etc). k is a 32B perm384_blockcipher key.
void ocb_encrypt(void *k, void *src, void *dst, int nbytes, void *mask, void *checksum) {
    uint8_t tmp[48];
    uint8_t *s8 = (uint8_t *)src;
    uint8_t *d8 = (uint8_t *)dst;
    while (nbytes >= 48) {
        prof_next_hash(mask);
        xor_block(checksum, s8, checksum);
        xor_block(mask, s8, tmp);
        prof_perm384_blockcipher(k, tmp, tmp);
        xor_block(mask, tmp, d8);
        s8 += 48;
        d8 += 48;
        nbytes -= 48;
    }
}

/*****************************Question 3 End************************************/
/*****************************Question 4 Start**********************************/
/**
 * The authentication tag is defined as E'(0, checksum) (ie, our tweaked
 * blockcipher with a tweak of 0). The encryption stage used tweaks 1, 2, 3,...,
 * but tweak 0 was not used and is reserved for tag generation. Since h(0) is
 * just the iv, E'(0, checksum) is just iv xor E(iv xor checksum). A 48 byte
 * tag is overkill, so only output the first 16 bytes.
 *
 * A real system would cache the iv from setup, but for simplicity we'll just
 * recreate it here as iv = E(nonce).
 *
 * Note: The nonce is 12 bytes and should be 10* padded into a 48 byte block
 * cipher input.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// dst = (k || 0) xor perm384((k || 0) xor src)
// k is 32B, src is 48B and dst is 48B
void prof_perm384_blockcipher(void *k, void *src, void *dst);

// Xor 48B from src1 and src2 and write result to dst
void xor_block(void *src1, void *src2, void *dst) {
    uint8_t *d = (uint8_t *)dst;
    uint8_t *s1 = (uint8_t *)src1;
    uint8_t *s2 = (uint8_t *)src2;
    for (int i=0; i<48; i++)
        d[i] = s1[i] ^ s2[i];
}

// Write first 16 bytes of h(0) xor E(h(0) xor checksum) to tag.
// h(0) is E(nonce||10*), k is a 32B perm384_blockcipher key.
void ocb_taggen(void *k, void *nonce, void *checksum, void *tag) {
    uint8_t h0[48];
    uint8_t tmp[48];
    uint8_t *tag8 = (uint8_t *)tag;
    // Set tmp to nonce || 10*, encipher with block cipher into h0
    memcpy(tmp, nonce, 12);
    memset(tmp+12, 0, 36);
    tmp[12] = 0x80;
    prof_perm384_blockcipher(k, tmp, h0);
    // Write first 16 bytes of h(0) xor E(h(0) xor checksum) to tag.
    xor_block(h0, checksum, tmp);
    prof_perm384_blockcipher(k, tmp, tmp);
    for (int i=0; i<16; i++)
        tag8[i] = tmp[i] ^ h0[i];
}

/*****************************Question 4 End************************************/
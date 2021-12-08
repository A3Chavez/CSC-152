/**
 * When using a cryptographic hash function H that is not susceptible to length-extension attacks, 
 * a simple way of generating MAC tags is H(K || M), where K is a random key shared by sender and 
 * receiver and || is concatenation. (A variation of this construction has been standardized for 
 * use with SHA-3 and is called KMAC.)
 *
 * For convenience, we're going to let users of our MAC specify how long the key and tag are that
 * they want to use. But for security, these choices should effect the value of the tag that is
 * generated, so in this problem you will write a function that computes a tag as
 *
 * H(<kbytes> || <tagbytes> || K || M)
 *
 * where <kbytes> and <tagbytes> are each one byte long and represent how many bytes long the key
 * and tag are, and || is concatenation.
 *
 * This is easy to compute if you have an incremental hash interface for H.
 *
 * 1. Make a two byte array and copy kbytes and tagbytes into the two elements.
 * 2. Use the hash incremental interface on the array, then use it on the key, then on the data M.
 * 3. In your final call generate the requested tag.
 * 
 * Implement the specified algorithm using the provided sponge_reset, sponge_update, and
 * sponge_final and the following interface.
 *
 * void sponge_taggen(void *k, int kbytes, void *m, int mbytes, void *tag, int tagbytes);
 *
 * NOTE: the provided hash functions require that blk be B+1 bytes long.
 *
 */

#include <stdint.h>
#include <stdlib.h>

// Sponge constants - R is subject to change
#define B 48     // Block length
#define R 16     // Rate: bytes of 'in' read per iteration
#define C (B-R)  // Capacity: part of state that 'in' doesn't touch

// sponge_reset initializes this blk for new hash
void sponge_reset(void *blk);

// sponge_update updates this blk with buffer pointed at by in
void sponge_update(void *blk, void *in, int inbytes);

// sponge_final updates this blk with buffer pointed at by in,
//   then completes the hash and outputs the result to out.
void sponge_final(void *blk, void *in, int inbytes, void *out, int outbytes);

// Computes H(<kbytes> || <tagbytes> || K || M) where <kbytes> and <tagbytes>
// are each one byte.
void sponge_taggen(void *k, int kbytes, void *m, int mbytes, void *tag, int tagbytes) {
    uint8_t blk[B+1];
    uint8_t tmp[2];
    tmp[0] = (uint8_t)kbytes;
    tmp[1] = (uint8_t)tagbytes;
    sponge_reset(blk);
    sponge_update(blk, tmp, 2);
    sponge_update(blk, k, kbytes);
    sponge_final(blk, m, mbytes, tag, tagbytes);
}

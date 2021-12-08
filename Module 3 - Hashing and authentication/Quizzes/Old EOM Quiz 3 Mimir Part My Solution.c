/**
 * Sometimes data is spread out into multiple pieces in memory and you want to consider
 * it as logically a single piece of data. For example, data FE AB 6C may be at one
 * address in memory and data 29 FF E3 may be at a different address in memory but you
 * may want to consider the data as if it were the unified data FE AB 6C 29 FF E3. An
 * algorithm that considers sacttered data to be logically one piece of data is called
 * a "gather" algorithm.
 *
 * Your job is to write a cryptographic gather hash using the interface to the perm384
 * sponge hash seen in homework, and the following interface.
 *
 * void sponge_gather(void *md, void **d, int *dlen, int n);
 *
 * The value n is how many chunks of data there are. dlen[i] is the number of bytes in
 * the i-th chunk, and d[i] is a pointer to the i-th chunk. You should consder chunks
 * 0, ..., n-1 as a single datum and write the 32-byte perm384 sponge hash of it to md.
 * In other words write to md the equivalent of sponge(d[0] || d[1] || ... || d[n-1])
 * where || is concatenation.
 *
 * One could malloc a temporary buffer big enough to hold all the data, copy all the data
 * to the buffer, and then hash the buffer. But, a more efficient way is to use the sponge
 * update function on each chunk of data before finalizing. Use the provided functions
 * sponge_reset, sponge_update, and sponge_final. NOTE: they only work with B+1 bytes for blk.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

// Writes 32-byte result of sponge(d[0] || ... || d[n-1]) to md
void sponge_gather(void *md, void **d, int *dlen, int n) {
  uint8_t blk[B+1];
  sponge_reset(blk);
  
  for(int i=0; i<n-1; i++) {
    sponge_update(blk, d[i], dlen[i]);
  }
  sponge_final(blk, d[n-1], dlen[n-1], md, 32);
}

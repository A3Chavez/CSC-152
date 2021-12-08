#include <stdint.h>
#include <string.h>

// Sponge constants - R is subject to change
#define B 48     // Block length
#define R 16     // Rate: bytes of 'in' read per iteration
#define C (B-R)  // Capacity: part of state that 'in' doesn't touch

// Implement the permutation for the 48 bytes beginning at p
void prof_perm384(void *p);

// blk points at a B+1 byte buffer. The first B bytes are the evolving
//   sponge block. Use of the final byte is optional.

// sponge_reset initializes this blk for new hash
void sponge_reset(void *blk) {
    memset(blk, 0, B+1);
}

// sponge_update updates this blk with buffer pointed at by in
void sponge_update(void *blk, void *in, int inbytes) {
    uint8_t *blk_p = blk;
    uint8_t *in_p = in;
    uint8_t inbuf = blk_p[B];      // How much is currently buffered
    int buf_remaining = R - inbuf; // How much space is left in the buffer

    // If some data is buffered and next data is big enough, process one blk
    if ((inbuf > 0) && (inbuf + inbytes >= R)) {
        for (int i=0; i<buf_remaining; i++)     // Xor buf_remaining bytes into blk
            blk_p[inbuf+i] ^= in_p[i];
        prof_perm384(blk);          // Apply permutation
        inbytes -= buf_remaining;
        in_p += buf_remaining;
        inbuf = 0;
    }
    while (inbytes >= R) {          // While there's still R bytes to read
        for (int i=0; i<R; i++)     // Xor R bytes into blk
            blk_p[i] ^= in_p[i];
        prof_perm384(blk);          // Apply permutation
        inbytes -= R;
        in_p += R;
    }
    // Buffer any remaining bytes and update buffer count
    for (int i=0; i<inbytes; i++)
        blk_p[inbuf+i] ^= in_p[i];
    blk_p[B] = inbuf + inbytes;
}

// sponge_final updates this blk with buffer pointed at by in,
//   then completes the hash and outputs the result to out.
void sponge_final(void *blk, void *in, int inbytes, void *out, int outbytes) {
    uint8_t *blk_p = blk;
    uint8_t *in_p = in;
    uint8_t *out_p = out;
    uint8_t inbuf = blk_p[B];      // How much is currently buffered
    int buf_remaining = R - inbuf; // How much space is left in the buffer

    // If some data is buffered and next data is big enough, process one blk
    if ((inbuf > 0) && (inbuf + inbytes >= R)) {
        for (int i=0; i<buf_remaining; i++)     // Xor buf_remaining bytes into blk
            blk_p[inbuf+i] ^= in_p[i];
        prof_perm384(blk);          // Apply permutation
        inbytes -= buf_remaining;
        in_p += buf_remaining;
    }
    while (inbytes >= R) {          // While there's still R bytes to read
        for (int i=0; i<R; i++)     // Xor R bytes into blk
            blk_p[i] ^= in_p[i];
        prof_perm384(blk_p);        // Apply permutation
        inbytes -= R;
        in_p += R;
    }
    for (int i=0; i<inbytes; i++)   // Xor any remaining bytes, inbytes < R
        blk_p[i] ^= in_p[i];
    blk_p[inbytes] ^= 0x80;         // 10*1 padding, okay if inbytes==R-1
    blk_p[R-1] ^= 0x01;
    prof_perm384(blk_p);            // Apply permutation
    while (outbytes >= R) {
        memcpy(out_p,blk_p,R);      // Output R bytes
        prof_perm384(blk_p);        // Apply permutation
        outbytes -= R;
        out_p += R;
    }
    memcpy(out_p,blk_p,outbytes);   // Output remaining bytes
}

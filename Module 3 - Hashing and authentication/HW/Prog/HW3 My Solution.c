#include <stdint.h>
#include <string.h>

// Sponge constants - R is subject to change
#define B 48     // blk length
#define R 16     // Rate: bytes of 'in' read per iteration
#define C (B-R)  // Capacity: part of state that 'in' doesn't touch

// Implement the permutation for the 48 bytes beginning at p
void prof_perm384(void *p);

// blk points at a B+1 byte buffer. The first B bytes are the evolving
//   sponge blk. Use of the final byte is optional.

// sponge_reset initializes this blk for new hash
void sponge_reset(void *blk) {
    uint8_t *p = (uint8_t *)blk;
    for(int i=0; i<B+1; i++) {
        p[i] = 0;
    }
}

// sponge_update updates this blk with buffer pointed at by in
void sponge_update(void *blk, void *in, int inbytes) {
    uint8_t *p = (uint8_t *)in;
    uint8_t *p8_blk = (uint8_t *)blk;
    while(inbytes >= R) {
        for(int i=0; i<R; i++) {
            p8_blk[p8_blk[B]] ^= p[i];
            p8_blk[B]++;
        
            if(p8_blk[B] == R) {
                prof_perm384(p8_blk);
                p8_blk[B] = 0;
            }
        }
        
        inbytes -= R;
        p += R;
    }
    
    for(int i=0; i<inbytes; i++) {
        p8_blk[p8_blk[B]] ^= p[i];
        p8_blk[B]++;
        
        if(p8_blk[B] == R) {
            prof_perm384(p8_blk);
            p8_blk[B] = 0;
        }
    }
}

// sponge_final updates this blk with buffer pointed at by in,
//   then completes the hash and outputs the result to out.
void sponge_final(void *blk, void *in, int inbytes, void *out, int outbytes) {
    uint8_t *p = (uint8_t *)in;
    uint8_t *p8_blk = (uint8_t *)blk;
    while(inbytes >= R) {
        for(int i=0; i<R; i++) {
            p8_blk[p8_blk[B]] ^= p[i];
            p8_blk[B]++;
        
            if(p8_blk[B] == R) {
                prof_perm384(p8_blk);
                p8_blk[B] = 0;
            }
        }
        
        inbytes -= R;
        p += R;
    }
    
    for(int i=0; i<inbytes; i++) {
        p8_blk[p8_blk[B]] ^= p[i];
        p8_blk[B]++;
        
        if(p8_blk[B] == R) {
            prof_perm384(p8_blk);
            p8_blk[B] = 0;
        }
    }
    
    p8_blk[p8_blk[B]] ^= 0x80;
    p8_blk[R-1] ^= 0x01;
            
    p = out;
    
    prof_perm384(p8_blk);
    
    while(outbytes >= R) {
        memcpy(p,p8_blk,R);
        prof_perm384(p8_blk);
        outbytes -= R;
        p += R;
    }
    
    memcpy(p, p8_blk, outbytes);
}

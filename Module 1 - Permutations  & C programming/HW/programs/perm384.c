#include <stdint.h>

void swap(uint32_t *a, uint32_t *b) {
    uint32_t newb = *a;
    uint32_t newa = *b;
    *a = newa;
    *b = newb;
}

// You can call this instead of pasting your own scramble, either way is ok
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

// Implement the permutation for the 48 bytes beginning at p
void perm384(void *p) {
  uint32_t *p32 = p;
  
  for(int rounds=0; rounds<24; rounds++) {
    scramble((p32+0),(p32+4),(p32+8));
    scramble((p32+1),(p32+5),(p32+9));
    scramble((p32+2),(p32+6),(p32+10));
    scramble((p32+3),(p32+7),(p32+11));
    
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
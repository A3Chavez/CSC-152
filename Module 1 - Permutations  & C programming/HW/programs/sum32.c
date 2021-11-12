#include <stdio.h>
#include <stdint.h>

// Read the four bytes begining at p and assemble them big-endian
uint32_t load_uint32_big(void *p) {
    uint32_t *p32 = p;
    uint32_t x = p32[0];
   // printf("x = %X\n\n",x);
    x = ((x>>24)&0xff) |        // move byte 3 to byte 0
      ((x<<8)&0xff0000) |     // move byte 1 to byte 2
      ((x>>8)&0xff00) |       // move byte 2 to byte 1
      ((x<<24)&0xff000000);   // byte 0 to byte 3
  //  printf("x = %X\n\n",x);
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

int main() {

	uint8_t mem[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02};
    uint32_t x = sum32(mem,8);
    printf("sum = %X\n\n", x);
    
    uint8_t mem2[] = {0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x02};
    uint32_t y = sum32(mem2,9);
    printf("sum = %X\n\n", y);

    return 0;
}
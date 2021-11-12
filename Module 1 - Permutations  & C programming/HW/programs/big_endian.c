#include <stdio.h>
#include <stdint.h>

int main() {
	uint8_t mem[4] = {4,3,2,1};
	uint32_t *p = (uint32_t *)mem;
    uint32_t x = p[0];
    printf("%X\n", x);
    x = ((x>>24)&0xff) |        // move byte 3 to byte 0
            ((x<<8)&0xff0000) |     // move byte 1 to byte 2
            ((x>>8)&0xff00) |       // move byte 2 to byte 1
            ((x<<24)&0xff000000);   // byte 0 to byte 3
    printf("%X\n", x);
}
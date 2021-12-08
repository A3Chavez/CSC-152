#include <stdint.h>
#include <stdio.h>

// return x^y mod p in O(log y) time. x and p must be lees than 2^32
uint64_t exp152(uint64_t x, uint64_t y, uint64_t p) {
  uint64_t res = 1; // initialize result
  
  x = x % p; // Update x if it is more than or
                // equal to p
  
  if (x == 0) return 0; // In case x is divisible by p;
 
  while (y > 0) {
    // If y is odd, multiply x with result
    if (y & 1)
        res = (res*x) % p;
 
    // y must be even now
    y = y>>1; // y = y/2
    x = (x*x) % p;
  }
  return res;
}

int main() {
    uint64_t x = 1473;
    uint64_t y = 3;
    uint64_t p = 2227;
    
    //printf("Test Result = %d\n", 3196010817 % 2227);
    
    uint64_t res = exp152(x,y,p);
    
    printf("Result = %d", exp152(x,y,p));
}
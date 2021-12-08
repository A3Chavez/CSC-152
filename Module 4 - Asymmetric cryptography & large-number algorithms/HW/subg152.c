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

/*
void subg152(uint64_t p, uint64_t q, uint64_t *subg) {
  uint64_t g = 0;
  uint64_t x = 0;
  int count = 0;
  
  //printf("p = %d\n", p);
  //printf("q = %d\n", q);
  //printf("subg[0] = %d\n", subg[0]);
  
  for(int i=0; i<(int)q; i++) {
    x = subg[i];
    printf("x = %d\n", x);
    count = 0;
    for(int j=1; j<=(int)q; j++) {
      count++;
      printf("Exp %d\n",exp152(x,j,p));
      if(exp152(x,(uint64_t)j,p) == (uint64_t)1) {
        if((uint64_t)count == q) {
          g = x;
          i = (int)q;
        }
        j = (int)q;
      }
    }
  }
  
  subg[0] = g;
  printf("subg[0] = %d\n\n\n\n", subg[0]);
  
  for(int i=1; i<(int)q; i++) {
    //subg[i] = exp152(g,exp152(subg[i],1,q),p);
    subg[i] = g * subg[i];
  }
  
  for(int i=0; i<(int)q; i++)
    printf("subg[%d] = %d\n",i,subg[i]);
}
*/

void subg152(uint64_t p, uint64_t q, uint64_t *subg) {
  uint64_t g = 0;
  uint64_t N = (p-1)/q;
  printf("N = %d\n",N);

  for(int i=2; i<(int)p; i++) {
    printf("exp152(i,N,p) = %d\n",exp152(i,N,p));
    if(exp152(i,N,p) != 1) {
      g = exp152(i,N,p);
      break;
    }
  }
  
  subg[0] = g;
  
  for(int i=1; i<=(int)q; i++) {
    subg[i-1] = exp152(subg[0], i, p);
  }
  
  for(int i=0; i<(int)q; i++)
    printf("subg[%d] = %d\n",i,subg[i]);
}

int main() {
    uint64_t subg[53];
    uint64_t p = 107;
    uint64_t q = 53;
    
    //printf("Test Result = %d\n", 3196010817 % 2227);
    
    subg152(p, q, subg);
}
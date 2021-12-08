/**
 * Implement x^y mod z in O(log y) time using the algorithm learned in class. Although all the parameters 
 * are given as uint64_t, you may assume that x and p are less than 232 (this allows you to do intermediate
 * multiplications without overflowing uint64_t's range).
 */
 
#include <stdint.h>

// return x^y mod p in O(log y) time. x and p must be less than 2^32
uint64_t exp152(uint64_t x, uint64_t y, uint64_t p) {
  uint64_t res = 1; // initialize result
  
  x %= p; // Update x if it's more than or equal to p
  
  if(x == 0)
    return 0; // in case x is divisible by p
    
  while (y > 0) {
    if(y & 1)
      res = (res * x) % p;  // y is odd, multiply x with result
    y = y >> 1; // y = y/2
    x = (x * x) % p;
  }
  
  return res;
}


/**
 * You have seen how you can use egcd to find multiplicative inverses mod n. Using egcd is the preferred
 * method in many cases. But, in the special case of finding multiplicative inverses mod p, where p is
 * prime, there is a simpler method.
 *
 * "Fermat's Little Theorem" tells us that if p is prime then xp mod p = x. This means xp-1 mod p = 1
 * and so (xp-2)(x) mod p = 1. Use this fact to find multiplicative inverses mod prime p. As before, x
 * and p will both be less than 232. Also, you may assume that 0 < x < p and that p is indeed prime.
 */

#include <stdint.h>

// return x^y mod p in O(log y) time. x and p must be lees than 2^32
uint64_t prof_exp152(uint64_t x, uint64_t y, uint64_t p);

// Return x^-1 mod p. Assumes 0 < x < p < 2^32 and p prime.
uint64_t inv152(uint64_t x, uint64_t p) {
  return prof_exp152(x, p-2, p);
}


 /**
  * Write a function that finds a subgroup of size q in Z*p when p and q are both prime and p = Nq+1 for
  * some positive integer N. Follow the procedure learned in class. The subg paramter is a pointer to
  * the base of an uint64_t array of exactly q elements. Once you find it, place your generator g in
  * subg[0] and then for all 0 < i < q, place g * subg[i-1] in subg[i] . This should result in subg[q-1]
  * being the only 1 in your list. You may assume 0 < q < p < 232 and that p and q are both prime with
  * p = Nq+1 for some positive integer N.
  */

#include <stdint.h>

// return x^y mod p in O(log y) time. x and p must be lees than 2^32
uint64_t prof_exp152(uint64_t x, uint64_t y, uint64_t p); 

// Fills pre-allocated subg array with subgroup of Z*p with q elements
// Assumes p < 2^32, p and q prime, p = Nq+1 for some positive integer N
void subg152(uint64_t p, uint64_t q, uint64_t *subg) {
  uint64_t g = 0;
  uint64_t N = (p-1)/q;

  for(int i=2; i<(int)p; i++) {
    if(prof_exp152(i,N,p) != 1) {
      g = prof_exp152(i,N,p);
      break;
    }
  }
  
  subg[0] = g;
  
  for(int i=1; i<=(int)q; i++) {
    subg[i-1] = prof_exp152(subg[0], i, p);
  }
}

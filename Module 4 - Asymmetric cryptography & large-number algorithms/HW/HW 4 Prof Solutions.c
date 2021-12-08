/**
 * Implement x^y mod z in O(log y) time using the algorithm learned in class. Although all the parameters 
 * are given as uint64_t, you may assume that x and p are less than 232 (this allows you to do intermediate
 * multiplications without overflowing uint64_t's range).
 */
 
#include <stdint.h>

static int bit(uint64_t x, int i) { return (x >> i) & 1; }

// return x^y mod p in O(log y) time. x and p must be lees than 2^32
uint64_t exp152(uint64_t x, uint64_t y, uint64_t p) {
    uint64_t acc = 1;
    for (int i=63; i>=0; i--) {
        acc = acc * acc % p;
        if (bit(y,i))
            acc = acc * x % p;
    }
    return acc;
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
    uint64_t x = 1;
    uint64_t g = 1;
    uint64_t N = (p-1)/q;
    while (g == 1) {
        x = x + 1;
        g = prof_exp152(x, N, p);
    }
    subg[0] = g;
    for (uint64_t i=1; i<q; i++) {
        subg[i] = g * subg[i-1] % p;
    } 
}

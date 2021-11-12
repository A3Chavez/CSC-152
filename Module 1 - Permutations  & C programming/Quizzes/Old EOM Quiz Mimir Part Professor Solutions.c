/**
 * Write a C function that is given a pointer to memory, the number of bytes of memory
 * the pointer points at, and two pointers to uint32_t. Your job is to treat the pointer
 * as if it were a pointer to an array of uint32_t, find the min and max values of the
 * array, and write the results to the locations pointed at by the uint32_t pointers.
 *
 * The number of bytes will be a positive multiple of 4. Do not worry about endianess
 * in this problem.
 *
 * Note the tester has command line options -Wall -Wextra -Werror -fsanitize=address
 * to help you catch errors.
 */

#include <stdint.h>

// Treat ptr as base address of a uint32_t array and place the min and
// max array values in memory where min and max point.
// num_bytes is a multiple of 4.
void min_max(void *ptr, int num_bytes, uint32_t *min, uint32_t *max) {
    uint32_t *ptr32 = (uint32_t *)ptr;
    uint32_t min_found = ptr32[0];
    uint32_t max_found = ptr32[0];
    for (int i=1; i<num_bytes/4; i++) {
        if (min_found > ptr32[i])
            min_found = ptr32[i];
        if (max_found < ptr32[i])
            max_found = ptr32[i];
    }
    *min = min_found;
    *max = max_found;
}
 
/**
 * Write a C function equal_halfs that takes a uint16_t as a parameter and returns
 * 0 if the first half of the parameter value is not the same as the second half of
 * the parameter value, and returns a non-zero value if they are the same. For
 * example, equal_halfs(0x2324) should return 0 and equal_halfs(0x2424) should
 * return any non-zero value.
 *
 * Note the tester has command line options -Wall -Wextra -Werror -fsanitize=address
 * to help you catch errors.
 */
 
 #include <stdint.h>

// Returns 0 iff first 8 bits of x differ from last 8 bits
int equal_halfs(uint16_t x) {
  uint8_t first = (uint8_t)(x >> 8);
  uint8_t second = (uint8_t)x;
  return first == second;
}

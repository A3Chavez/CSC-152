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
  int i= 0;
  uint32_t min_num = UINT32_MAX; 
  uint32_t max_num = 0; 
  while(4*i < num_bytes){
    uint32_t a = *(ptr32 + i);
    if(a > max_num) max_num = a; 
      if(a < min_num) min_num = a;
        i += 1; // Move in front
  }
  *min = min_num; 
  *max = max_num; 
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
  uint16_t a = x & 0x00FF;
  uint16_t b = (x & 0xFF00) >> 8;
  
  if(a == b) {
    return 1;
  }
  else {
    return 0;
  }
}
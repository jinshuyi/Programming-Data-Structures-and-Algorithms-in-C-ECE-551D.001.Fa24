#include <stdio.h>

unsigned power_helper(unsigned x, unsigned y, unsigned accum) {
  if (y == 0) {
    return accum;
  }
  if (y % 2 == 0) {
    return power_helper(x * x, y / 2, accum);
  }
  return power_helper(x, y - 1, x * accum);
}

unsigned power(unsigned x, unsigned y) {
  return power_helper(x, y, 1);
}

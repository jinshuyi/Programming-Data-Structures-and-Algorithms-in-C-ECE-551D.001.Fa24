#include <stdio.h>
#include <stdlib.h>
size_t max(int * array, size_t n) {
  if (n == 0) {
    return 0;
  }

  size_t maxi = 1;
  size_t curr = 1;

  int number = array[0];

  for (int i = 0; i < n; i++) {
    if (array[i] > number) {
      curr++;
    }
    else {
      curr = 1;
    }
    if (maxi < curr) {
      maxi = curr;
    }
    number = array[i];
  }
  return maxi;
}

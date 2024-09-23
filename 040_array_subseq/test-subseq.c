#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n);

int main() {
  int arr1[] = {1, 2, 3, 4};
  size_t len1 = maxSeq(arr1, 0);
  if (len1 != 0) {
    return EXIT_FAILURE;
  }
  int arr2[] = {5};
  size_t len2 = maxSeq(arr2, 1);
  if (len2 != 1) {
    return EXIT_FAILURE;
  }
  int arr3[] = {3, 3, 3, 3, 3};
  size_t len3 = maxSeq(arr3, 5);
  if (len3 != 1) {
    return EXIT_FAILURE;
  }
  int arr4[] = {1, 2, 3, 4, 5};
  size_t len4 = maxSeq(arr4, 5);
  if (len4 != 5) {
    return EXIT_FAILURE;
  }
  int arr5[] = {7, 6, 5, 4, 3};
  size_t len5 = maxSeq(arr5, 5);
  if (len5 != 1) {
    return EXIT_FAILURE;
  }
  int arr6[] = {0, 0, 1, 3, 5, 7, 0, 4, 6, 9};
  size_t len6 = maxSeq(arr6, 10);
  if (len6 != 5) {
    return EXIT_FAILURE;
  }
  int arr7[] = {1, 2, 3, 1, 2, 3, 4};
  size_t len7 = maxSeq(arr7, 7);
  if (len7 != 4) {
    return EXIT_FAILURE;
  }
  int arr8[] = {-1, -2, -3, -4, -3};
  size_t len8 = maxSeq(arr8, 5);
  if (len8 != 2) {
    return EXIT_FAILURE;
  }
  int arr9[] = {-2, -1, 0, 2, -1};
  size_t len9 = maxSeq(arr9, 5);
  if (len9 != 4) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

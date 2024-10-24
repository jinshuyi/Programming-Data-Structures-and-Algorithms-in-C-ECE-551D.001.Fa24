#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <words file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  catarray_t * cats = readCategories(argv[1]);
  printWords(cats);
  freeCatarray(cats);
  return EXIT_SUCCESS;
}

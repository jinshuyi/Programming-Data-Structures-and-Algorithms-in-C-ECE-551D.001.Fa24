#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <words file> <story file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  catarray_t * cats = readCategories(argv[1]);
  processStory(argv[2], cats, 0);
  freeCatarray(cats);
  return EXIT_SUCCESS;
}

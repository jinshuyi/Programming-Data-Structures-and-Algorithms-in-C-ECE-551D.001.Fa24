#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage: %s [-n] <words file> <story file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  int noReuse = 0;
  int wordFileIndex = 1;
  if (argc == 4) {
    if (strcmp(argv[1], "-n") == 0) {
      noReuse = 1;
      wordFileIndex = 2;
    }
    else {
      fprintf(stderr, "Usage: %s [-n] <words file> <story file>\n", argv[0]);
      return EXIT_FAILURE;
    }
  }

  catarray_t * cats = readCategories(argv[wordFileIndex]);
  processStory(argv[wordFileIndex + 1], cats, noReuse);
  freeCatarray(cats);
  return EXIT_SUCCESS;
}

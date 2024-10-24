#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <story file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  processStory(argv[1], NULL, 0);
  return EXIT_SUCCESS;
}

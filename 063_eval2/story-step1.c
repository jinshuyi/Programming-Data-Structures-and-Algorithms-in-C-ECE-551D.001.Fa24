#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <story template>\n", argv[0]);
    return EXIT_FAILURE;
  }
  read_template(argv[1], NULL, 0);  // No category handling yet
  return EXIT_SUCCESS;
}

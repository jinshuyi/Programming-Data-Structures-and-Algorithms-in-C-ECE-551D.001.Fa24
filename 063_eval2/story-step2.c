#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <word file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  // Read the words from the file into cats
  catarray_t * cats = read_word_file(argv[1]);

  // Free the memory associated with cats
  freeCatarray(cats);
  return EXIT_SUCCESS;
}

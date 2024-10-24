#include <stdio.h>
#include <stdlib.h>

#include "provided.h"
#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s words.txt\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }

  catarray_t * cats = readWords(f);
  printWords(cats);
  freeCatarray(cats);

  fclose(f);
  return EXIT_SUCCESS;
}

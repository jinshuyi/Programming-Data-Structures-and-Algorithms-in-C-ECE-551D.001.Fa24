#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <word_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  catarray_t cats = read_categories(f);
  fclose(f);

  printWords(&cats);

  return EXIT_SUCCESS;
}

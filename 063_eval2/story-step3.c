#include <stdio.h>
#include <stdlib.h>

#include "provided.h"
#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <words file> <story template>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open words file");
    return EXIT_FAILURE;
  }

  catarray_t * cats = readWords2(f);
  fclose(f);

  read_template_with_backreference(argv[2], cats, 1);  // allow_repeat is 1 for step 3

  return EXIT_SUCCESS;
}

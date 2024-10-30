
#include <stdio.h>
#include <stdlib.h>

#include "provided.h"
#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "error");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open words file");
    return EXIT_FAILURE;
  }
  //first read words
  catarray_t * cats = readWords2(f);
  fclose(f);
  // allow_repeat is 1 for step 3
  read_template_with_backreference_3(argv[2], cats, 1);
  //free to make valgridn clean
  freeCatarray2(cats);
  return EXIT_SUCCESS;
}

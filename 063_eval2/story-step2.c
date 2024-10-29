#include <stdio.h>
#include <stdlib.h>

#include "provided.h"
#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Error");
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }

  //use readWords2
  catarray_t * cats = readWords2(f);

  //use printWords (given in provided.h to print)
  printWords(cats);
  //free to avoid memory leak
  freeCatarray2(cats);

  fclose(f);
  return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"
#include "rand_story.h"

int main(int argc, char ** argv) {
  //The default setting is to allow duplicates
  int allow_repeat = 1;
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }
  //chekc if -n occcur
  if (strcmp(argv[1], "-n") == 0) {
    allow_repeat = 0;  //set it to 0
    argv++;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open words file");
    return EXIT_FAILURE;
  }

  catarray_t * cats = readWords2(f);
  fclose(f);
  //use allow_repeat to detect
  read_template_with_backreference(argv[2], cats, allow_repeat);

  freeCatarray2(cats);
  return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "errorr");
    return EXIT_FAILURE;
  }
  // set allow_repeat to 0(used for step34)
  //use read_template(which included replace_ category to replace and print)
  read_template(argv[1], NULL, 0);
  return EXIT_SUCCESS;
}

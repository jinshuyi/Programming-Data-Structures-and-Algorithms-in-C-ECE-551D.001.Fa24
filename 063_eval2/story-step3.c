#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <words file> <story template>\n", argv[0]);
    return EXIT_FAILURE;
  }
  catarray_t * cats = read_word_file(argv[1]);
  read_template(argv[2], cats, 0);  // Allow repeated words
  return EXIT_SUCCESS;
}

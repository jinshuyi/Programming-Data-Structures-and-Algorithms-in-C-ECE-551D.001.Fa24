#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  int allow_repeat = 1;  // By default, allow repeats
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage: %s [-n] <words file> <story template>\n", argv[0]);
    return EXIT_FAILURE;
  }
  if (argc == 4 && strcmp(argv[1], "-n") == 0) {
    allow_repeat = 0;
    argv++;
  }
  catarray_t * cats = read_word_file(argv[1]);
  read_template(argv[2], cats, allow_repeat);
  return EXIT_SUCCESS;
}

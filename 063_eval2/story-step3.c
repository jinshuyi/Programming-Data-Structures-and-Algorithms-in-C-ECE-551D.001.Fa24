#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <words_file> <story_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  catarray_t * cats = readWords(argv[1]);

  FILE * f = fopen(argv[2], "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot open file %s\n", argv[2]);
    return EXIT_FAILURE;
  }

  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) >= 0) {
    replaceBlanks(line, cats, 0, NULL, NULL);
  }

  free(line);
  fclose(f);
  freeCatarray(cats);
  return EXIT_SUCCESS;
}

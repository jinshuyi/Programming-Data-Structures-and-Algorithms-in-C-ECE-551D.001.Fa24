#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <story_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot open file %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) >= 0) {
    printf("%s", line);
  }

  free(line);
  fclose(f);
  return EXIT_SUCCESS;
}

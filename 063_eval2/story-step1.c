#include <stdio.h>

#include "rand_story.h"
int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <story_template_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  replace_blanks_with_words(f, NULL, 0);

  fclose(f);
  return EXIT_SUCCESS;
}

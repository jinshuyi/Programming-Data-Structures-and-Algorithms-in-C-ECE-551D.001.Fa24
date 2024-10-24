#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage: %s [-n] <word file> <story template>\n", argv[0]);
    return EXIT_FAILURE;
  }

  int no_repeat = 0;
  if (argc == 4 && strcmp(argv[1], "-n") == 0) {
    no_repeat = 1;
    argv++;  // Shift arguments
  }

  // Read the words from the file into cats
  catarray_t * cats = read_word_file(argv[1]);

  // Read the story template
  FILE * f = fopen(argv[2], "r");
  if (f == NULL) {
    perror("Could not open story template file");
    return EXIT_FAILURE;
  }

  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) != -1) {
    replace_category(line, cats);  // Pass the cats to the function
  }

  free(line);
  fclose(f);
  freeCatarray(cats);  // Free the memory associated with cats
  return EXIT_SUCCESS;
}

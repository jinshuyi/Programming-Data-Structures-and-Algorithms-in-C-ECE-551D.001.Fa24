#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage: %s [-n] <words file> <story template file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int no_reuse = 0;
  int arg_idx = 1;
  if (strcmp(argv[arg_idx], "-n") == 0) {
    no_reuse = 1;
    arg_idx++;
  }

  FILE * words_file = fopen(argv[arg_idx], "r");
  if (words_file == NULL) {
    perror("Could not open words file");
    exit(EXIT_FAILURE);
  }

  catarray_t * cats = read_words(words_file);
  fclose(words_file);

  FILE * story_file = fopen(argv[arg_idx + 1], "r");
  if (story_file == NULL) {
    perror("Could not open story template file");
    free_catarray(cats);
    exit(EXIT_FAILURE);
  }

  category_t used_words = {.words = NULL, .n_words = 0};
  parse_template(story_file, cats, &used_words, no_reuse);
  fclose(story_file);

  free_category(&used_words);
  free_catarray(cats);

  return EXIT_SUCCESS;
}

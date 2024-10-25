#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <words file> <story template file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE * words_file = fopen(argv[1], "r");
  if (words_file == NULL) {
    perror("Could not open words file");
    exit(EXIT_FAILURE);
  }

  catarray_t * cats = read_words(words_file);
  fclose(words_file);

  FILE * story_file = fopen(argv[2], "r");
  if (story_file == NULL) {
    perror("Could not open story template file");
    free_catarray(cats);
    exit(EXIT_FAILURE);
  }

  category_t used_words = {.words = NULL, .n_words = 0};
  parse_template(story_file, cats, &used_words, 0);
  fclose(story_file);

  free_category(&used_words);
  free_catarray(cats);

  return EXIT_SUCCESS;
}

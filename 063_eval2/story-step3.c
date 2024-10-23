#include "rand_story.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <word_file> <story_template_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE * wordFile = fopen(argv[1], "r");
  if (wordFile == NULL) {
    perror("Error opening word file");
    return EXIT_FAILURE;
  }

  FILE * storyFile = fopen(argv[2], "r");
  if (storyFile == NULL) {
    perror("Error opening story file");
    fclose(wordFile);
    return EXIT_FAILURE;
  }

  catarray_t cats = read_categories(wordFile);
  fclose(wordFile);

  replace_blanks_with_words(storyFile, &cats, 0);

  fclose(storyFile);
  return EXIT_SUCCESS;
}

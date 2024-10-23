#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3 && argc != 4) {
    fprintf(stderr, "Usage: %s [-n] <word_file> <story_template_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  int noReuse = 0;
  char * wordFileName;
  char * storyFileName;

  if (argc == 4) {
    if (strcmp(argv[1], "-n") == 0) {
      noReuse = 1;
      wordFileName = argv[2];
      storyFileName = argv[3];
    }
    else {
      fprintf(stderr, "Usage: %s [-n] <word_file> <story_template_file>\n", argv[0]);
      return EXIT_FAILURE;
    }
  }
  else {
    wordFileName = argv[1];
    storyFileName = argv[2];
  }

  FILE * wordFile = fopen(wordFileName, "r");
  if (wordFile == NULL) {
    perror("Error opening word file");
    return EXIT_FAILURE;
  }

  FILE * storyFile = fopen(storyFileName, "r");
  if (storyFile == NULL) {
    perror("Error opening story file");
    fclose(wordFile);
    return EXIT_FAILURE;
  }

  catarray_t cats = read_categories(wordFile);
  fclose(wordFile);

  replace_blanks_with_words(storyFile, &cats, noReuse);

  fclose(storyFile);
  return EXIT_SUCCESS;
}

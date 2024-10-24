#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s words.txt story_template.txt\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE * wordsFile = fopen(argv[1], "r");
  if (wordsFile == NULL) {
    perror("Could not open words file");
    return EXIT_FAILURE;
  }

  FILE * storyFile = fopen(argv[2], "r");
  if (storyFile == NULL) {
    perror("Could not open story template file");
    fclose(wordsFile);
    return EXIT_FAILURE;
  }

  catarray_t * cats = readWords(wordsFile);
  parseTemplate(storyFile, cats, 1);
  freeCatarray(cats);

  fclose(wordsFile);
  fclose(storyFile);
  return EXIT_SUCCESS;
}

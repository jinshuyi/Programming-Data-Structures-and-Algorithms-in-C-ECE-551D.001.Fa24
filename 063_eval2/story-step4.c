#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  int allowReuse = 1;
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage: %s [-n] words.txt story_template.txt\n", argv[0]);
    return EXIT_FAILURE;
  }

  int wordFileIndex = 1;
  int storyFileIndex = 2;
  if (argc == 4) {
    if (strcmp(argv[1], "-n") == 0) {
      allowReuse = 0;
      wordFileIndex = 2;
      storyFileIndex = 3;
    }
    else {
      fprintf(stderr, "Error: unknown option '%s'\n", argv[1]);
      return EXIT_FAILURE;
    }
  }

  FILE * wordsFile = fopen(argv[wordFileIndex], "r");
  if (wordsFile == NULL) {
    perror("Could not open words file");
    return EXIT_FAILURE;
  }

  FILE * storyFile = fopen(argv[storyFileIndex], "r");
  if (storyFile == NULL) {
    perror("Could not open story template file");
    fclose(wordsFile);
    return EXIT_FAILURE;
  }

  catarray_t * cats = readWords(wordsFile);
  parseTemplate(storyFile, cats, allowReuse);
  freeCatarray(cats);

  fclose(wordsFile);
  fclose(storyFile);
  return EXIT_SUCCESS;
}

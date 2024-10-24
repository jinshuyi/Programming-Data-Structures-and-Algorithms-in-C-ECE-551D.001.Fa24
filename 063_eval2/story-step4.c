#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3 && (argc != 4 || strcmp(argv[1], "-n") != 0)) {
    fprintf(stderr, "Usage: %s [-n] <words_file> <story_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  int noReuse = 0;
  const char * wordsFile;
  const char * storyFile;

  if (argc == 4) {
    noReuse = 1;
    wordsFile = argv[2];
    storyFile = argv[3];
  }
  else {
    wordsFile = argv[1];
    storyFile = argv[2];
  }

  catarray_t * cats = readWords(wordsFile);

  FILE * f = fopen(storyFile, "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot open file %s\n", storyFile);
    return EXIT_FAILURE;
  }

  char ** usedWords = NULL;
  size_t usedCount = 0;

  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) >= 0) {
    replaceBlanks(line, cats, noReuse, usedWords, &usedCount);
  }

  free(line);
  fclose(f);
  freeCatarray(cats);

  for (size_t i = 0; i < usedCount; i++) {
    free(usedWords[i]);
  }
  free(usedWords);

  return EXIT_SUCCESS;
}

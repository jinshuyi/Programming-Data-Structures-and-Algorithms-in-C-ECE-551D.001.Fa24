#include "rand_story.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"

// Helper function to replace blanks with words
void replaceBlanks(char * line,
                   catarray_t * cats,
                   int noReuse,
                   char ** usedWords,
                   size_t * usedCount) {
  char * start = line;
  while ((start = strchr(start, '_')) != NULL) {
    char * end = strchr(start + 1, '_');
    if (end == NULL) {
      fprintf(stderr, "Invalid template: unmatched underscore.\n");
      exit(EXIT_FAILURE);
    }

    *end = '\0';
    const char * word = chooseWord(start + 1, cats);
    printf("%s", word);
    *end = '_';
    start = end + 1;
  }
}

// Process the story template and replace the blanks
void processStory(const char * filename, catarray_t * cats, int noReuse) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  char * line = NULL;
  size_t size = 0;
  while (getline(&line, &size, f) != -1) {
    replaceBlanks(line, cats, noReuse, NULL, NULL);
    printf("%s", line);
  }
  free(line);
  fclose(f);
}

// Read categories and words from a file
catarray_t * readCategories(const char * filename) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  catarray_t * cats = malloc(sizeof(*cats));
  cats->arr = NULL;
  cats->n = 0;

  char * line = NULL;
  size_t size = 0;
  while (getline(&line, &size, f) != -1) {
    char * colon = strchr(line, ':');
    if (colon == NULL) {
      fprintf(stderr, "Invalid format: no colon found.\n");
      exit(EXIT_FAILURE);
    }

    *colon = '\0';
    category_t * cat = NULL;
    for (size_t i = 0; i < cats->n; i++) {
      if (strcmp(cats->arr[i].name, line) == 0) {
        cat = &cats->arr[i];
        break;
      }
    }

    if (cat == NULL) {
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cat = &cats->arr[cats->n++];
      cat->name = strdup(line);
      cat->words = NULL;
      cat->n_words = 0;
    }

    char * word = colon + 1;
    word[strcspn(word, "\n")] = '\0';
    cat->words = realloc(cat->words, (cat->n_words + 1) * sizeof(*cat->words));
    cat->words[cat->n_words++] = strdup(word);
  }

  free(line);
  fclose(f);
  return cats;
}

// Free memory for catarray_t
void freeCatarray(catarray_t * cats) {
  for (size_t i = 0; i < cats->n; i++) {
    for (size_t j = 0; j < cats->arr[i].n_words; j++) {
      free(cats->arr[i].words[j]);
    }
    free(cats->arr[i].words);
    free(cats->arr[i].name);
  }
  free(cats->arr);
  free(cats);
}

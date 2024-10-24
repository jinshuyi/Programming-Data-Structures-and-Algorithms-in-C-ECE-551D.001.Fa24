#include "rand_story.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"

// Helper function to replace underscores with words
void replaceUnderscore(char * line,
                       size_t * index,
                       catarray_t * cats,
                       int allowReuse,
                       char ** usedWords,
                       size_t * usedCount) {
  size_t start = *index;
  while (line[*index] != '_' && line[*index] != '\0') {
    (*index)++;
  }
  if (line[*index] == '\0') {
    fprintf(stderr, "Error: unmatched underscore in template.\n");
    exit(EXIT_FAILURE);
  }
  line[*index] = '\0';
  char * category = line + start + 1;

  // Handle backreference case
  if (category[0] >= '1' && category[0] <= '9') {
    int backRef = atoi(category);
    if (backRef > *usedCount) {
      fprintf(stderr, "Error: backreference to an unused word.\n");
      exit(EXIT_FAILURE);
    }
    printf("%s", usedWords[*usedCount - backRef]);
  }
  else {
    const char * word = chooseWord(category, cats);
    printf("%s", word);
    usedWords[*usedCount] = strdup(word);
    (*usedCount)++;
  }

  (*index)++;
}

// Function to parse and replace placeholders in the template with a word from categories
void parseTemplate(FILE * f, catarray_t * cats, int allowReuse) {
  char * line = NULL;
  size_t len = 0;
  char * usedWords[100];
  size_t usedCount = 0;

  while (getline(&line, &len, f) != -1) {
    size_t i = 0;
    while (line[i] != '\0') {
      if (line[i] == '_') {
        replaceUnderscore(line, &i, cats, allowReuse, usedWords, &usedCount);
      }
      else {
        printf("%c", line[i]);
        i++;
      }
    }
  }
  free(line);
}

// Function to read categories and words from file and return a catarray_t
catarray_t * readWords(FILE * f) {
  catarray_t * catArr = malloc(sizeof(*catArr));
  catArr->arr = NULL;
  catArr->n = 0;
  char * line = NULL;
  size_t len = 0;

  while (getline(&line, &len, f) != -1) {
    char * colon = strchr(line, ':');
    if (colon == NULL) {
      fprintf(stderr, "Error: invalid format in word file.\n");
      free(line);
      exit(EXIT_FAILURE);
    }
    *colon = '\0';
    char * category = line;
    char * word = colon + 1;
    word[strlen(word) - 1] = '\0';  // remove newline

    size_t catIdx = 0;
    while (catIdx < catArr->n && strcmp(catArr->arr[catIdx].name, category) != 0) {
      catIdx++;
    }

    if (catIdx == catArr->n) {
      catArr->arr = realloc(catArr->arr, (catArr->n + 1) * sizeof(*catArr->arr));
      catArr->arr[catArr->n].name = strdup(category);
      catArr->arr[catArr->n].words = NULL;
      catArr->arr[catArr->n].n_words = 0;
      catArr->n++;
    }

    category_t * cat = &catArr->arr[catIdx];
    cat->words = realloc(cat->words, (cat->n_words + 1) * sizeof(*cat->words));
    cat->words[cat->n_words] = strdup(word);
    cat->n_words++;
  }
  free(line);
  return catArr;
}

// Function to free allocated memory for catarray_t
void freeCatarray(catarray_t * catArr) {
  for (size_t i = 0; i < catArr->n; i++) {
    for (size_t j = 0; j < catArr->arr[i].n_words; j++) {
      free(catArr->arr[i].words[j]);
    }
    free(catArr->arr[i].words);
    free(catArr->arr[i].name);
  }
  free(catArr->arr);
  free(catArr);
}

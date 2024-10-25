#include "rand_story.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"

void replace_category(char * line, catarray_t * cats, int allow_repeat) {
  char * start = line;
  while ((start = strchr(start, '_')) != NULL) {
    char * end = strchr(start + 1, '_');
    if (end == NULL) {
      fprintf(stderr, "Unmatched underscore in story template\n");
      exit(EXIT_FAILURE);
    }

    // Extract the category
    size_t cat_len = end - start - 1;
    char category[cat_len + 1];
    strncpy(category, start + 1, cat_len);
    category[cat_len] = '\0';

    const char * word = chooseWord(category, cats);

    // Print everything up to the category and replace with the word
    *start = '\0';
    printf("%s%s", line, word);
    start = end + 1;
    line = start;
  }
  // Print remaining part of the line
  printf("%s", line);
}

void read_template(const char * filename, catarray_t * cats, int allow_repeat) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    perror("Could not open template file");
    exit(EXIT_FAILURE);
  }
  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) != -1) {
    replace_category(line, cats, allow_repeat);
  }
  free(line);
  fclose(f);
}

catarray_t * read_word_file(const char * filename) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    perror("Could not open word file");
    exit(EXIT_FAILURE);
  }

  catarray_t * cats = malloc(sizeof(catarray_t));
  cats->arr = NULL;
  cats->n = 0;

  char * line = NULL;
  size_t sz = 0;

  while (getline(&line, &sz, f) != -1) {
    // Check for colon
    char * colon = strchr(line, ':');
    if (colon == NULL) {
      fprintf(stderr, "Invalid line format: %s", line);
      exit(EXIT_FAILURE);
    }

    // Extract category name
    size_t cat_len = colon - line;
    line[cat_len] = '\0';  // Null-terminate category name
    char * category_name = strdup(line);

    // Count words in this category
    char ** words = NULL;
    size_t n_words = 0;

    // Read words
    char * word = colon + 1;  // Skip the colon
    while (word && *word != '\0' && *word != '\n') {
      // Trim whitespace
      while (*word == ' ')
        word++;
      if (*word != '\0' && *word != '\n') {
        words = realloc(words, (n_words + 1) * sizeof(char *));
        words[n_words] = strdup(word);
        n_words++;
      }
      // Find the next word
      word = strchr(word, ' ');
      if (word) {
        *word = '\0';  // Null-terminate the word
        word++;
      }
    }

    // Add to the categories array
    cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(category_t));
    cats->arr[cats->n].name = category_name;
    cats->arr[cats->n].words = words;
    cats->arr[cats->n].n_words = n_words;
    cats->n++;
  }

  free(line);
  fclose(f);
  return cats;
}

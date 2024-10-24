#include "rand_story.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"

// Function to replace categories in a line with words from cats
void replace_category(char * line, catarray_t * cats) {
  char * start = line;
  while ((start = strchr(start, '_')) != NULL) {
    char * end = strchr(start + 1, '_');
    if (end == NULL) {
      fprintf(stderr, "Unmatched underscore in story template\n");
      exit(EXIT_FAILURE);
    }

    // Extract the category name
    size_t cat_len = end - start - 1;
    char category[cat_len + 1];
    strncpy(category, start + 1, cat_len);
    category[cat_len] = '\0';

    // Choose a word from the category, passing the cats struct (NOT NULL)
    const char * word = chooseWord(category, cats);

    // Replace the category with the chosen word
    *start = '\0';               // Terminate the current string before the category
    printf("%s%s", line, word);  // Print the line with the replaced word
    start = end + 1;
    line = start;
  }
  printf("%s", line);  // Print the remaining part of the line
}

// Function to read the word list from a file into a catarray_t structure
catarray_t * read_word_file(const char * filename) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    perror("Could not open word file");
    exit(EXIT_FAILURE);
  }

  // Initialize the cats structure
  catarray_t * cats = malloc(sizeof(*cats));
  cats->n = 0;
  cats->arr = NULL;

  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) != -1) {
    // Parse the line into category and word
    char * colon = strchr(line, ':');
    if (colon == NULL) {
      fprintf(stderr, "Malformed line in word file: %s", line);
      exit(EXIT_FAILURE);
    }
    *colon = '\0';
    char * category = line;
    char * word = colon + 1;
    word[strcspn(word, "\n")] = '\0';  // Remove newline

    // Add the word to the category in cats
    int found = 0;
    for (size_t i = 0; i < cats->n; i++) {
      if (strcmp(cats->arr[i].name, category) == 0) {
        cats->arr[i].words = realloc(
            cats->arr[i].words, (cats->arr[i].n_words + 1) * sizeof(*cats->arr[i].words));
        cats->arr[i].words[cats->arr[i].n_words] = strdup(word);
        cats->arr[i].n_words++;
        found = 1;
        break;
      }
    }
    if (!found) {
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cats->arr[cats->n].name = strdup(category);
      cats->arr[cats->n].words = malloc(sizeof(*cats->arr[cats->n].words));
      cats->arr[cats->n].words[0] = strdup(word);
      cats->arr[cats->n].n_words = 1;
      cats->n++;
    }
  }

  free(line);
  fclose(f);
  return cats;
}

// Function to free memory allocated for catarray_t
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

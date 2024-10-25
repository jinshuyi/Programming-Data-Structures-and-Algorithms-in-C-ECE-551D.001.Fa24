#include "rand_story.h"

#include <ctype.h>
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

//step2
catarray_t * readWords2(FILE * f) {
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
void freeCatarray2(catarray_t * catArr) {
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

//step3,4 final function
void parse_template(FILE * file,
                    catarray_t * cats,
                    category_t * used_words,
                    int no_reuse) {
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, file)) != -1) {
    char * p = line;

    while (*p != '\0') {
      if (*p == '_') {
        // find the next underscore to extract category name
        char * end = strchr(p + 1, '_');
        if (end == NULL) {
          fprintf(stderr, "Error: unmatched underscore in template\n");
          exit(EXIT_FAILURE);
        }

        // extract category name
        size_t cat_len = end - p - 1;
        char category[cat_len + 1];
        strncpy(category, p + 1, cat_len);
        category[cat_len] = '\0';

        // check if category is a backreference
        const char * word = NULL;
        if (isdigit(category[0])) {
          int ref_index = atoi(category);
          if (ref_index <= 0 || ref_index > used_words->n_words) {
            fprintf(stderr, "Error: invalid backreference\n");
            exit(EXIT_FAILURE);
          }
          word = used_words->words[used_words->n_words - ref_index];
        }
        else {
          word = chooseWord(category, cats);
          if (no_reuse) {
            add_used_word(cats, category, word);
          }
        }

        // store the word in used_words array
        used_words->words[used_words->n_words] = strdup(word);
        used_words->n_words++;

        // print the selected word
        printf("%s", word);
        p = end + 1;
      }
      else {
        putchar(*p++);
      }
    }
  }

  free(line);
}

void add_used_word(catarray_t * cats, const char * category, const char * word) {
  for (size_t i = 0; i < cats->n; i++) {
    if (strcmp(cats->arr[i].name, category) == 0) {
      // Find word index
      for (size_t j = 0; j < cats->arr[i].n_words; j++) {
        if (strcmp(cats->arr[i].words[j], word) == 0) {
          // Shift words down to remove word
          free(cats->arr[i].words[j]);
          for (size_t k = j; k < cats->arr[i].n_words - 1; k++) {
            cats->arr[i].words[k] = cats->arr[i].words[k + 1];
          }
          cats->arr[i].n_words--;
          return;
        }
      }
    }
  }
}

catarray_t * read_words(FILE * file) {
  catarray_t * cats = malloc(sizeof(*cats));
  cats->arr = NULL;
  cats->n = 0;

  char * line = NULL;
  size_t len = 0;

  while (getline(&line, &len, file) != -1) {
    char * sep = strchr(line, ':');
    if (sep == NULL) {
      fprintf(stderr, "Error: invalid format in words file\n");
      exit(EXIT_FAILURE);
    }

    *sep = '\0';
    char * category = line;
    char * word = sep + 1;

    word[strcspn(word, "\n")] = '\0';

    int cat_index = -1;
    for (size_t i = 0; i < cats->n; i++) {
      if (strcmp(cats->arr[i].name, category) == 0) {
        cat_index = i;
        break;
      }
    }

    if (cat_index == -1) {
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cats->arr[cats->n].name = strdup(category);
      cats->arr[cats->n].words = NULL;
      cats->arr[cats->n].n_words = 0;
      cat_index = cats->n;
      cats->n++;
    }

    category_t * cat = &cats->arr[cat_index];
    cat->words = realloc(cat->words, (cat->n_words + 1) * sizeof(*cat->words));
    cat->words[cat->n_words] = strdup(word);
    cat->n_words++;
  }

  free(line);
  return cats;
}

void free_catarray(catarray_t * cats) {
  for (size_t i = 0; i < cats->n; i++) {
    free(cats->arr[i].name);
    for (size_t j = 0; j < cats->arr[i].n_words; j++) {
      free(cats->arr[i].words[j]);
    }
    free(cats->arr[i].words);
  }
  free(cats->arr);
  free(cats);
}

void free_category(category_t * used_words) {
  for (size_t i = 0; i < used_words->n_words; i++) {
    free(used_words->words[i]);
  }
  free(used_words->words);
}

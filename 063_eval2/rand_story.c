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
// 读取 words 文件并解析成 catarray_t 结构
catarray_t * readWords(FILE * f) {
  catarray_t * cats = malloc(sizeof(*cats));
  cats->arr = NULL;
  cats->n = 0;

  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) >= 0) {
    // 查找 ':' 符号以分离类别名和单词
    char * colon = strchr(line, ':');
    if (colon == NULL) {
      fprintf(stderr, "Invalid line format\n");
      exit(EXIT_FAILURE);
    }
    *colon = '\0';
    char * category = line;
    char * word = colon + 1;
    word[strcspn(word, "\n")] = '\0';  // 移除换行符

    // 查找或创建类别
    size_t i;
    for (i = 0; i < cats->n; i++) {
      if (strcmp(cats->arr[i].name, category) == 0) {
        break;
      }
    }
    if (i == cats->n) {  // 新类别
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cats->arr[i].name = strdup(category);
      cats->arr[i].words = NULL;
      cats->arr[i].n_words = 0;
      cats->n++;
    }

    // 添加单词到该类别
    category_t * cat = &cats->arr[i];
    cat->words = realloc(cat->words, (cat->n_words + 1) * sizeof(*cat->words));
    cat->words[cat->n_words] = strdup(word);
    cat->n_words++;
  }
  free(line);
  return cats;
}

// 释放 catarray_t 结构的内存
void freeCatarray(catarray_t * cats) {
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

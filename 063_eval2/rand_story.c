#include "rand_story.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"

// 读取文件并构建词汇数组
catarray_t * readWords(const char * filename) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  catarray_t * cats = malloc(sizeof(*cats));
  cats->arr = NULL;
  cats->n = 0;

  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) >= 0) {
    char * colon = strchr(line, ':');
    if (colon == NULL) {
      fprintf(stderr, "Invalid format in file %s\n", filename);
      exit(EXIT_FAILURE);
    }

    *colon = '\0';
    char * category = line;
    char * word = colon + 1;
    char * newline = strchr(word, '\n');
    if (newline != NULL) {
      *newline = '\0';
    }

    // 查找是否已有此类别
    int found = 0;
    for (size_t i = 0; i < cats->n; i++) {
      if (strcmp(cats->arr[i].words[0], category) == 0) {
        cats->arr[i].words = realloc(
            cats->arr[i].words, (cats->arr[i].n_words + 1) * sizeof(*cats->arr[i].words));
        cats->arr[i].words[cats->arr[i].n_words] = strdup(word);
        cats->arr[i].n_words++;
        found = 1;
        break;
      }
    }

    // 新类别
    if (!found) {
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cats->arr[cats->n].words = malloc(sizeof(*cats->arr[cats->n].words));
      cats->arr[cats->n].words[0] = strdup(category);
      cats->arr[cats->n].n_words = 1;
      cats->n++;
    }
  }

  free(line);
  fclose(f);
  return cats;
}

// 释放词汇数组的内存
void freeCatarray(catarray_t * cats) {
  for (size_t i = 0; i < cats->n; i++) {
    for (size_t j = 0; j < cats->arr[i].n_words; j++) {
      free(cats->arr[i].words[j]);
    }
    free(cats->arr[i].words);
  }
  free(cats->arr);
  free(cats);
}
// 替换模板中的空白项
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

    *start = '\0';
    printf("%s", line);
    *start = '_';

    *end = '\0';
    const char * word = chooseWord(start + 1, cats);
    printf("%s", word);
    *end = '_';

    // 更新usedWords列表
    usedWords[*usedCount] = strdup(word);
    (*usedCount)++;

    line = end + 1;
  }
  printf("%s", line);
}

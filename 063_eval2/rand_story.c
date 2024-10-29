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
#define MAX_CATEGORIES 10     // 假设最多有10个类别
#define MAX_REPLACEMENTS 100  // 假设最多有100个占位符

void replace_category_with_backreference(char * line,
                                         catarray_t * cats,
                                         int allow_repeat) {
  char * start = line;
  size_t count = 0;                              // 用于记录占位符数量
  char * replacements[MAX_CATEGORIES] = {NULL};  // 存储每个类别最后使用的词
  int category_count = 0;                        // 类别计数

  while ((start = strchr(start, '_')) != NULL) {
    char * end = strchr(start + 1, '_');
    if (end == NULL) {
      fprintf(stderr, "Unmatched underscore in story template\n");
      exit(EXIT_FAILURE);
    }

    // 提取类别名称
    size_t cat_len = end - start - 1;
    char category[cat_len + 1];
    strncpy(category, start + 1, cat_len);
    category[cat_len] = '\0';

    const char * word = NULL;

    // 检查是否为回溯引用
    if (isdigit(category[0])) {
      int backref_index = atoi(category);
      if (backref_index < 1 || backref_index > count) {
        fprintf(stderr, "Invalid back reference: %s\n", category);
        exit(EXIT_FAILURE);
      }
      // 用已存储的词替换
      word = replacements[count - backref_index];
    }
    else {
      // 检查类别是否已经被使用
      for (int i = 0; i < category_count; i++) {
        if (strcmp(replacements[i], category) == 0) {
          word = replacements[i];
          break;
        }
      }

      // 如果没有找到，则随机选择
      if (word == NULL) {
        word = chooseWord(category, cats);
        if (word != NULL) {
          replacements[category_count++] = strdup(word);  // 存储类别最后使用的词
        }
      }
    }

    // 打印一切到类别并替换为词
    *start = '\0';
    printf("%s%s", line, word);
    start = end + 1;
    line = start;
    count++;
  }
  // 打印剩余部分
  printf("%s", line);
}

void read_template_with_backreference(const char * filename,
                                      catarray_t * cats,
                                      int allow_repeat) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    perror("Could not open template file");
    exit(EXIT_FAILURE);
  }
  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) != -1) {
    replace_category_with_backreference(line, cats, allow_repeat);
  }
  free(line);
  fclose(f);
}

#include "rand_story.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 读取文件中的类别和单词
catarray_t read_categories(FILE * f) {
  char * line = NULL;
  size_t sz = 0;
  ssize_t len;
  catarray_t cats;
  cats.n = 0;
  cats.arr = NULL;

  while ((len = getline(&line, &sz, f)) != -1) {
    char * colon = strchr(line, ':');
    if (colon == NULL) {
      fprintf(stderr, "Error: Incorrect format in word file\n");
      exit(EXIT_FAILURE);
    }

    *colon = '\0';  // 分割类别和单词
    char * category = line;
    char * word = colon + 1;
    word[strcspn(word, "\n")] = '\0';  // 去掉换行符

    // 查找是否已有该类别
    size_t i;
    for (i = 0; i < cats.n; i++) {
      if (strcmp(cats.arr[i].name, category) == 0) {
        break;
      }
    }

    // 如果没有找到该类别，创建新的类别
    if (i == cats.n) {
      cats.arr = realloc(cats.arr, (cats.n + 1) * sizeof(*cats.arr));
      cats.arr[cats.n].name = strdup(category);
      cats.arr[cats.n].words = NULL;
      cats.arr[cats.n].n_words = 0;
      cats.n++;
    }

    // 将单词加入类别
    cats.arr[i].words = realloc(cats.arr[i].words,
                                (cats.arr[i].n_words + 1) * sizeof(*cats.arr[i].words));
    cats.arr[i].words[cats.arr[i].n_words] = strdup(word);
    cats.arr[i].n_words++;
  }

  free(line);
  return cats;
}

// 替换故事模板中的占位符
void replace_blanks_with_words(FILE * f, catarray_t * cats, int noReuse) {
  char * line = NULL;
  size_t sz = 0;
  ssize_t len;
  char * usedWords[100];  // 假设最多用100个单词
  size_t usedCount = 0;

  while ((len = getline(&line, &sz, f)) != -1) {
    char * pos = line;
    while (*pos != '\0') {
      if (*pos == '_') {
        char * end = strchr(pos + 1, '_');
        if (end == NULL) {
          fprintf(stderr, "Error: Mismatched underscores\n");
          exit(EXIT_FAILURE);
        }

        *end = '\0';  // 暂时将结尾的下划线置为字符串结尾

        int ref = atoi(pos + 1);
        if (ref > 0 && ref <= (int)usedCount) {
          printf("%s", usedWords[usedCount - ref]);  // 引用之前的单词
        }
        else {
          const char * word = chooseWord(pos + 1, cats);  // 调用provided.h中的chooseWord
          usedWords[usedCount] = strdup(word);            // 记录使用过的单词
          usedCount++;
          printf("%s", word);
        }

        pos = end + 1;  // 跳过当前的占位符
      }
      else {
        putchar(*pos);
        pos++;
      }
    }
  }
  free(line);
}

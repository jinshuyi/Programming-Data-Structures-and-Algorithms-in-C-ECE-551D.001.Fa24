#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"
#include "rand_story.h"

// 用于存储已使用单词的结构
typedef struct usedWords_t {
  char ** words;
  size_t n_words;
} usedWords_t;

// 将单词添加到已使用单词列表
void addUsedWord(usedWords_t * usedWords, const char * word) {
  usedWords->words =
      realloc(usedWords->words, (usedWords->n_words + 1) * sizeof(*usedWords->words));
  usedWords->words[usedWords->n_words] = strdup(word);
  usedWords->n_words++;
}

// 根据索引获取已使用的单词
const char * getUsedWord(usedWords_t * usedWords, int index) {
  if (index > 0 && index <= usedWords->n_words) {
    return usedWords->words[usedWords->n_words - index];
  }
  fprintf(stderr, "Invalid backreference\n");
  exit(EXIT_FAILURE);
}

// 处理模板文件并替换占位符
void processTemplate(FILE * templateFile, catarray_t * catArray) {
  usedWords_t usedWords = {NULL, 0};  // 初始化已使用的单词
  char * line = NULL;
  size_t sz = 0;

  while (getline(&line, &sz, templateFile) >= 0) {
    char * ptr = line;
    while (*ptr != '\0') {
      if (*ptr == '_') {
        ptr++;
        char * end = strchr(ptr, '_');
        if (end == NULL) {
          fprintf(stderr, "Invalid template format\n");
          exit(EXIT_FAILURE);
        }
        *end = '\0';

        const char * replacement = NULL;
        if (isdigit(*ptr)) {  // 数字 -> 引用
          int refIndex = atoi(ptr);
          replacement = getUsedWord(&usedWords, refIndex);
        }
        else {  // 类别 -> 随机选择
          replacement = chooseWord(ptr, catArray);
          addUsedWord(&usedWords, replacement);
        }

        printf("%s", replacement);
        ptr = end + 1;  // 移动指针到下一个字符
      }
      else {
        putchar(*ptr);
        ptr++;
      }
    }
    putchar('\n');  // 确保每行结束后换行
  }

  free(line);
  for (size_t i = 0; i < usedWords.n_words; i++) {
    free(usedWords.words[i]);
  }
  free(usedWords.words);
}

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <words file> <template file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE * wordsFile = fopen(argv[1], "r");
  if (wordsFile == NULL) {
    perror("Could not open words file");
    return EXIT_FAILURE;
  }

  FILE * templateFile = fopen(argv[2], "r");
  if (templateFile == NULL) {
    perror("Could not open template file");
    fclose(wordsFile);
    return EXIT_FAILURE;
  }

  catarray_t * catArray = readWords(wordsFile);
  fclose(wordsFile);

  processTemplate(templateFile, catArray);
  fclose(templateFile);

  freeCatarray(catArray);
  return EXIT_SUCCESS;
}

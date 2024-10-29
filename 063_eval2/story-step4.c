#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3 && argc != 4) {
    fprintf(stderr, "Usage: %s [-n] words.txt story.txt\n", argv[0]);
    return EXIT_FAILURE;
  }

  int allow_repeat = 1;  // 默认允许重复
  const char * words_filename;
  const char * template_filename;

  if (argc == 4 && strcmp(argv[1], "-n") == 0) {
    allow_repeat = 0;  // 如果有 "-n"，则不允许重复
    words_filename = argv[2];
    template_filename = argv[3];
  }
  else if (argc == 3) {
    words_filename = argv[1];
    template_filename = argv[2];
  }
  else {
    fprintf(stderr, "Invalid usage\n");
    return EXIT_FAILURE;
  }

  // 打开单词文件
  FILE * f = fopen(words_filename, "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }

  // 读取单词并存储在 cats 中
  catarray_t * cats = readWords2(f);
  fclose(f);

  // 初始化 used_words 用于引用跟踪
  category_t used_words = {NULL, NULL, 0};

  // 读取模板文件并替换类别标记，根据 allow_repeat 控制重复使用
  read_template34(template_filename, cats, allow_repeat, &used_words);

  // 释放内存
  freeCatarray2(cats);
  for (size_t i = 0; i < used_words.n_words; i++) {
    free(used_words.words[i]);
  }
  free(used_words.words);

  return EXIT_SUCCESS;
}

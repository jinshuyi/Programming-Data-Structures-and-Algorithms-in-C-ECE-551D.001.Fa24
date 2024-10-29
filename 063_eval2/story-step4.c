// story-step4.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"
#include "rand_story.h"

int main(int argc, char ** argv) {
  int allow_repeat = 1;  // 默认允许重复

  // 解析命令行参数
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage: %s [-n] <words file> <story template>\n", argv[0]);
    return EXIT_FAILURE;
  }

  // 检查是否有 "-n" 参数
  if (strcmp(argv[1], "-n") == 0) {
    allow_repeat = 0;  // 不允许重复
    argv++;            // 向后移动参数
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open words file");
    return EXIT_FAILURE;
  }

  catarray_t * cats = readWords2(f);
  fclose(f);

  read_template_with_backreference(argv[2], cats, allow_repeat);  // 使用允许重复参数

  return EXIT_SUCCESS;
}

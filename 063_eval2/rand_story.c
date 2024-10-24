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

    // Choose a word from the category
    const char * word = chooseWord(category, cats);  // 修正选择单词逻辑

    // Replace category with the chosen word
    *start = '\0';               // 结束当前字符串
    printf("%s%s", line, word);  // 打印到单词
    start = end + 1;
    line = start;
  }
  printf("%s", line);  // 打印剩余部分
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
  // Logic to read and store categories and words
  // This will store words in catarray_t structure

  // Allocate memory for catarray_t, and populate it by reading file lines
  // Implementation left for story-step2

  fclose(f);
  return NULL;
}

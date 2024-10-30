#include "rand_story.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"

//step1
void replace_category(char * line, catarray_t * cats, int allow_repeat) {
  char * start = line;
  while ((start = strchr(start, '_')) != NULL) {
    char * end = strchr(start + 1, '_');
    if (end == NULL) {
      fprintf(stderr, "erorr\n");
      exit(EXIT_FAILURE);
    }
    // Determine the category
    size_t cat_len = end - start - 1;
    char category[cat_len + 1];
    strncpy(category, start + 1, cat_len);
    category[cat_len] = '\0';
    //use given function chooseWord int he provided.h
    const char * word = chooseWord(category, cats);

    // replace the word with "cat" and print
    *start = '\0';
    printf("%s%s", line, word);
    start = end + 1;
    line = start;
  }
  // print the other content of the reamining
  printf("%s", line);
}

//utlize the function replace_category, add the function open file and read words
void read_template(const char * filename, catarray_t * cats, int allow_repeat) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    perror("can not open file");
    exit(EXIT_FAILURE);
  }
  char * line = NULL;
  size_t sz = 0;
  //use getline function
  while (getline(&line, &sz, f) >= 0) {
    replace_category(line, cats, allow_repeat);
  }
  free(line);
  fclose(f);
}

//step2

//function readWords2 to read the file
catarray_t * readWords2(FILE * f) {
  catarray_t * catArr = malloc(sizeof(*catArr));
  catArr->arr = NULL;
  catArr->n = 0;
  char * line = NULL;
  size_t len = 0;

  while (getline(&line, &len, f) >= 0) {
    char * colon = strchr(line, ':');
    if (colon == NULL) {
      fprintf(stderr, "Error: invalid format in word file.\n");
      free(line);
      exit(EXIT_FAILURE);
    }
    *colon = '\0';  // end symbol
    char * category = line;
    char * word = colon + 1;
    word[strlen(word) - 1] = '\0';  //remove the newline generated

    //
    //
    //Check if the category already exists in catArr
    size_t catIdx = 0;
    while (catIdx < catArr->n && strcmp(catArr->arr[catIdx].name, category) != 0) {
      catIdx++;
    }
    //If the category does not exist, create a new category
    if (catIdx == catArr->n) {
      catArr->arr = realloc(catArr->arr, (catArr->n + 1) * sizeof(*catArr->arr));
      catArr->arr[catArr->n].name = strdup(category);
      catArr->arr[catArr->n].words = NULL;
      catArr->arr[catArr->n].n_words = 0;
      catArr->n++;
    }
    //Get the current category
    category_t * cat = &catArr->arr[catIdx];
    cat->words = realloc(cat->words, (cat->n_words + 1) * sizeof(*cat->words));
    cat->words[cat->n_words] = strdup(word);
    cat->n_words++;
  }
  free(line);
  return catArr;
}

// free alllocated memory for catarray_t
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
#define MAX_CATEGORIES 100   //Assume a maximum of 100 categories
#define MAX_USED_WORDS 1000  //Assume that a maximum of 100 used words are stored
char * used_words[MAX_USED_WORDS];
int used_count = 0;

// new replace_category_with_backreference function
void replace_category_with_backreference_3(char * line,
                                           catarray_t * cats,
                                           int allow_repeat) {
  char * start = line;
  size_t count = 0;
  char * replacements[MAX_CATEGORIES] = {NULL};
  int category_count = 0;
  while ((start = strchr(start, '_')) != NULL) {
    char * end = strchr(start + 1, '_');
    if (end == NULL) {
      fprintf(stderr, "Unmatched underscore in story template\n");
      exit(EXIT_FAILURE);
    }
    //Extract class name
    size_t cat_len = end - start - 1;
    char category[cat_len + 1];
    strncpy(category, start + 1, cat_len);
    category[cat_len] = '\0';

    const char * word = NULL;
    //
    //Check for backtracking reference
    if (isdigit(category[0])) {
      int backref_index = atoi(category);
      if (backref_index < 1 || backref_index > count) {
        fprintf(stderr, "Invalid back reference: %s\n", category);
        exit(EXIT_FAILURE);
      }
      //Replace with a stored word
      word = replacements[count - backref_index];
    }
    else {
      //
      for (int i = 0; i < category_count; i++) {
        if (strcmp(replacements[i], category) == 0) {
          word = replacements[i];
          break;
        }
      }

      //
      if (word == NULL) {
        word = chooseWord(category, cats);

        //
        while (word != NULL) {
          int already_used = 0;

          //
          if (!allow_repeat) {
            for (int i = 0; i < used_count; i++) {
              if (strcmp(used_words[i], word) == 0) {
                already_used = 1;
                break;
              }
            }
          }

          //
          if (allow_repeat || !already_used) {
            replacements[category_count++] = strdup(word);
            used_words[used_count++] = strdup(word);
            break;
          }
          else {
            //
            word = chooseWord(category, cats);
          }
        }
      }
    }

    *start = '\0';
    printf("%s%s", line, word);
    start = end + 1;
    line = start;
    count++;
  }

  //Print the remaining part
  printf("%s", line);

  //Freeing up memory
  for (int i = 0; i < category_count; i++) {
    free(replacements[i]);
  }
  for (int i = 0; i < used_count; i++) {
    free(used_words[i]);
  }
  used_count = 0;
}

//Reading template files and handling backreferences
void read_template_with_backreference_3(const char * filename,
                                        catarray_t * cats,
                                        int allow_repeat) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    perror("can'topen template file");
    exit(EXIT_FAILURE);
  }
  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) >= 0) {
    replace_category_with_backreference_3(line, cats, allow_repeat);
  }
  free(line);
  fclose(f);
}

//Important comment:Because when I add the "free(used_words[i]);"comment

//the same as read_template_with_backreference_3
void read_template_with_backreference_4(const char * filename,
                                        catarray_t * cats,
                                        int allow_repeat) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    perror("can'topen template file");
    exit(EXIT_FAILURE);
  }
  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) >= 0) {
    replace_category_with_backreference_4(line, cats, allow_repeat);
  }
  free(line);
  fclose(f);
}

void replace_category_with_backreference_4(char * line,
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

        // 检查是否已使用过
        while (word != NULL) {
          int already_used = 0;

          // 仅当不允许重复时，检查已使用的单词
          if (!allow_repeat) {
            for (int i = 0; i < used_count; i++) {
              if (strcmp(used_words[i], word) == 0) {
                already_used = 1;
                break;
              }
            }
          }

          // 如果未使用过，记录已使用单词
          if (allow_repeat || !already_used) {
            replacements[category_count++] = strdup(word);  // 存储类别最后使用的词
            used_words[used_count++] = strdup(word);        // 记录已使用的单词
            break;  // 找到未使用的单词，退出循环
          }
          else {
            // 重新选择单词
            word = chooseWord(category, cats);
          }
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

  for (int i = 0; i < category_count; i++) {
    free(replacements[i]);
  }
}

#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>

#include "provided.h"

//step1
void replace_category(char * line, catarray_t * cats, int allow_repeat);
void read_template(const char * filename, catarray_t * cats, int allow_repeat);

//step2
catarray_t * readWords2(FILE * f);
void freeCatarray2(catarray_t * catArr);

//step3,4
typedef struct {
  char ** used_words;
  size_t count;
} used_words_t;
void replace_category_with_backreference(char * line,
                                         catarray_t * cats,
                                         int allow_repeat);
void read_template_with_backreference(const char * filename,
                                      catarray_t * cats,
                                      int allow_repeat);
#endif

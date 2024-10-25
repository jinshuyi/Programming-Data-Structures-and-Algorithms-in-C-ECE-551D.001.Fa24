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
void parse_template(FILE * file,
                    catarray_t * cats,
                    category_t * used_words,
                    int no_reuse);
void add_used_word(catarray_t * cats, const char * category, const char * word);
catarray_t * read_words(FILE * file);
void free_catarray(catarray_t * cats);
void free_category(category_t * used_words);

#endif

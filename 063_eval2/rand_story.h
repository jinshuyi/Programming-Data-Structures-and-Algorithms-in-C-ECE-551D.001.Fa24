#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>

#include "provided.h"

void replace_category(char * line, catarray_t * cats, int allow_repeat);

void read_template(const char * filename, catarray_t * cats, int allow_repeat);

catarray_t * read_word_file(const char * filename);

#endif

#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>

#include "provided.h"

// Function declarations
void replace_category(char * line, catarray_t * cats);
catarray_t * read_word_file(const char * filename);
void freeCatarray(catarray_t * cats);

#endif

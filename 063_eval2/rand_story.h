#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__
#include "provided.h"
#include "stdio.h"

//any functions you want your main to use

catarray_t read_categories(FILE * f);

void replace_blanks_with_words(FILE * f, catarray_t * cats, int noReuse);
#endif

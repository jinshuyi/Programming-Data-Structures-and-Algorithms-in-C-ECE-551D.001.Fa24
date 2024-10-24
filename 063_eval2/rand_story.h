#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>

#include "provided.h"

void replaceBlanks(char * line,
                   catarray_t * cats,
                   int noReuse,
                   char ** usedWords,
                   size_t * usedCount);
catarray_t * readWords(const char * filename);

// Free the memory allocated for the catarray_t structure
void freeCatarray(catarray_t * cats);

#endif

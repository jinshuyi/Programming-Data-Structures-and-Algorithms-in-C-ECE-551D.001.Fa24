#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>

#include "provided.h"

// Read the story template from a file and process it, replacing blanks
void processStory(const char * filename, catarray_t * cats, int noReuse);

// Read the words and categories from a file and store them into a catarray_t
catarray_t * readCategories(const char * filename);

// Free the memory allocated for the catarray_t structure
void freeCatarray(catarray_t * cats);

#endif

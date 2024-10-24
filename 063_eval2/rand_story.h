#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>

#include "provided.h"

// Function to parse and replace placeholders in the template with a word from categories
void parseTemplate(FILE * f, catarray_t * cats, int allowReuse);

// Function to read categories and words from file and return a catarray_t
catarray_t * readWords(FILE * f);

// Function to free allocated memory for catarray_t
void freeCatarray(catarray_t * catArr);

#endif

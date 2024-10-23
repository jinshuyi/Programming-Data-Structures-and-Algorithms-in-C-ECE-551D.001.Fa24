#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include "provided.h"

//any functions you want your main to use
//  returrn the content as the  string
char * readTemplate(const char * filename);
//use to replace all the placeholders
void replaceBlanks(char * template);

#endif

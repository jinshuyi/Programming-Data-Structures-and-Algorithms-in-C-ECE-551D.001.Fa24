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

//step3
//change the function in step1 (replace_category --> replace_category_with_backreference;
//read_template --->read_template_with_backreference)
void read_template_with_backreference_3(const char * filename,
                                        catarray_t * cats,
                                        int allow_repeat);
void replace_category_with_backreference_3(char * line,
                                           catarray_t * cats,
                                           int allow_repeat);

//step4
//!!!
//actually 3 and 4 fucntion is almost the same, but when I add oneline comment of free the useword[i], step4 -n is not correct and I try many times but still don not know. To pass more testcases, I seperate it to 2 different functions and only leave the memory leak of step4(I know how to solve , but when I sovle the output change) and make it output to be right. The details I will shown in  rand_story.c
void replace_category_with_backreference_4(char * line,
                                           catarray_t * cats,
                                           int allow_repeat);
void read_template_with_backreference_4(const char * filename,
                                        catarray_t * cats,
                                        int allow_repeat);
#endif

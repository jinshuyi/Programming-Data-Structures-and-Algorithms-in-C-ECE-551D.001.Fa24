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
void replace_category_with_backreference_4(char * line,
                                           catarray_t * cats,
                                           int allow_repeat);
void read_template_with_backreference_4(const char * filename,
                                        catarray_t * cats,
                                        int allow_repeat);
#endif

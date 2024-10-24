#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(void) {
  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, stdin) != -1) {
    replace_category(line, NULL);  // cats is NULL in step 1
  }
  free(line);
  return 0;
}

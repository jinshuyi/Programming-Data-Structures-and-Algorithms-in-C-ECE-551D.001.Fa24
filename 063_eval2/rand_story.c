#include "rand_story.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//read and return the string type
char * readTemplate(const char * filename) {
  // open the file
  FILE * file = fopen(filename, "r");
  //ensure the file is not balnk
  if (file == NULL) {
    fprintf(stderr, "Error, the file can not be openned");
    exit(EXIT_FAILURE);
  }
  // intialize the variables needed
  char * template = NULL;
  size_t sz = 0;
  char * line = NULL;
  size_t total_len = 0;

  // use getline to read the file,just as AOP's practice question
  while (getline(&line, &sz, file) >= 0) {
    total_len += strlen(line);

    // Reallocate the memory,+1 for '\0'
    template = realloc(template, total_len + 1);
    //ensure the template is correct
    if (template == NULL) {
      fprintf(stderr, "The Memory allocation has   failed");
      exit(EXIT_FAILURE);
    }

    //use strcat to append
    strcat(template, line);
  }

  free(line);
  fclose(file);

  if (template == NULL) {
    fprintf(stderr, "File is empty or could not be read");
    exit(EXIT_FAILURE);
  }

  // Return the template( full story)
  return template;
}

//replace the placeholder to cat
void replaceWithCat(char * template) {
  // new char a poniter to reutrn
  char * pos = template;

  // find placeholders
  while ((pos = strchr(pos, '_')) != NULL) {
    // Find next '_'
    char * end = strchr(pos + 1, '_');
    //ensure if threre exists unmatched '_'
    if (end == NULL) {
      fprintf(stderr, "Erorr\n");
      exit(EXIT_FAILURE);
    }

    //  print the first '_' and part before the blank
    *pos = '\0';
    printf("%s", template);
    // Replace with "cat"
    printf("cat");

    // Skip past the placeholder
    pos = end + 1;
    template = pos;  // Continue the next
  }

  // Print the  remaining part  of the contents
  printf("%s", template);
}

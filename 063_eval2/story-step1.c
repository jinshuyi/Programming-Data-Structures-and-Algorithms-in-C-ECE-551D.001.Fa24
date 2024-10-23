#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char * argv[]) {
  //command line argument chekcing
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <story_template_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  //read from the file
  char * story_template = readTemplate(argv[1]);

  if (story_template == NULL) {
    fprintf(stderr, "Error,Failed to read the story");

    return EXIT_FAILURE;
  }

  //replace with cats
  readTemplate(story_template);

  //free
  free(story_template);

  return EXIT_SUCCESS;
}

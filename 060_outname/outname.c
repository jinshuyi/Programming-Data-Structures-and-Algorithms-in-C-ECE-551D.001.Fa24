#include "outname.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * computeOutputFileName(const char * inputName) {
  //WRITE ME
  size_t len = strlen(inputName);
  char * outName = malloc(len + 8);  // for ".counts\0"

  if (outName == NULL) {
    return NULL;
  }

  strcpy(outName, inputName);
  strcat(outName, ".counts");
  return outName;
}

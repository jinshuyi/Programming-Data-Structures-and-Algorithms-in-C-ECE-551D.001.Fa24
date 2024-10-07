#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering of the strings
//in qsort. You do not need to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void processFile(FILE * f) {
  char ** lines = NULL;
  char * curr = NULL;
  size_t sz;
  size_t n = 0;

  while (getline(&curr, &sz, f) >= 0) {
    lines = (char **)realloc(lines, (n + 1) * sizeof(*lines));
    if (lines == NULL) {
      perror("Failed to allocate memory");
      exit(EXIT_FAILURE);
    }
    lines[n] = curr;
    curr = NULL;  //so getline allocates a new string next iteration
    n++;
  }
  free(curr);

  if (n > 0) {
    sortData(lines, n);
    for (size_t i = 0; i < n; i++) {
      printf("%s", lines[i]);
      free(lines[i]);
    }
    free(lines);
  }
}

int main(int argc, char ** argv) {
  //WRITE YOUR CODE HERE!
  if (argc == 1) {
    // No arguments provided; read from stdin
    processFile(stdin);
  }
  else {
    // Process each input file
    for (int i = 1; i < argc; i++) {
      FILE * f = fopen(argv[i], "r");
      if (f == NULL) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
      }
      processFile(f);
      fclose(f);
    }
  }
  return EXIT_SUCCESS;
}

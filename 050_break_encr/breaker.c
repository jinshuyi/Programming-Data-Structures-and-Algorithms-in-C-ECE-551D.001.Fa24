#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void freq(FILE * f, int * frequency) {
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (isalpha(c)) {
      c = tolower(c);
      frequency[c - 'a']++;
    }
  }
}

int analysis(int * frequency) {
  int e = 0, max = 0;
  for (int i = 0; i < 26; i++) {
    if (frequency[i] > max) {
      e = i;
      max = frequency[i];
    }
  }
  return (e + 22) % 26;
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: encrypt key inputFileName\n");
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }

  int frequency[26] = {0};
  freq(f, frequency);
  int e = analysis(frequency);

  printf("%d\n", e);

  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

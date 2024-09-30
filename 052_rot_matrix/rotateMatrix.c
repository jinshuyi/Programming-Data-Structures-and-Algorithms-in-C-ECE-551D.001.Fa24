#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX_SIZE 10

void validateAndExtractMatrix(FILE * file, char matrix[MATRIX_SIZE][MATRIX_SIZE]);
void printRotatedMatrix(char matrix[MATRIX_SIZE][MATRIX_SIZE]);

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE * file = fopen(argv[1], "r");
  if (!file) {
    perror("Cannot open the file");
    return EXIT_FAILURE;
  }

  char matrix[MATRIX_SIZE][MATRIX_SIZE];
  validateAndExtractMatrix(file, matrix);
  printRotatedMatrix(matrix);

  if (fclose(file) != 0) {
    perror("Cannot close the file");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void validateAndExtractMatrix(FILE * file, char matrix[MATRIX_SIZE][MATRIX_SIZE]) {
  char line[MATRIX_SIZE + 2];  // +2 for newline and null-terminator

  for (int i = 0; i < MATRIX_SIZE; i++) {
    if (fgets(line, sizeof(line), file) == NULL || line[0] == '\n' ||
        line[MATRIX_SIZE] != '\n') {
      fprintf(stderr, "Input file does not have a valid 10x10 matrix format.\n");
      exit(EXIT_FAILURE);
    }

    for (int j = 0; j < MATRIX_SIZE; j++) {
      matrix[j][MATRIX_SIZE - 1 - i] = line[j];
    }
  }
}

void printRotatedMatrix(char matrix[MATRIX_SIZE][MATRIX_SIZE]) {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX_SIZE 10

void rotateMatrix(char matrix[MATRIX_SIZE][MATRIX_SIZE],
                  char rotated[MATRIX_SIZE][MATRIX_SIZE]) {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      rotated[j][MATRIX_SIZE - 1 - i] = matrix[i][j];
    }
  }
}

int main(int argc, char * argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE * file = fopen(argv[1], "r");
  if (file == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  char matrix[MATRIX_SIZE][MATRIX_SIZE];
  char rotated[MATRIX_SIZE][MATRIX_SIZE];
  char line[MATRIX_SIZE + 2];  // 10 characters + newline + null terminator

  int lineCount = 0;
  while (fgets(line, sizeof(line), file) != NULL) {
    lineCount++;

    // Check if the line has exactly 10 characters followed by a newline
    if (strlen(line) != MATRIX_SIZE + 1 || line[MATRIX_SIZE] != '\n') {
      fprintf(stderr,
              "Invalid line format at line %d: Expected 10 characters followed by a "
              "newline\n",
              lineCount);
      fclose(file);
      return EXIT_FAILURE;
    }

    // Copy the first 10 characters into the matrix
    strncpy(matrix[lineCount - 1], line, MATRIX_SIZE);
  }

  if (lineCount != MATRIX_SIZE) {
    fprintf(stderr, "Invalid number of lines: Expected 10, but got %d\n", lineCount);
    fclose(file);
    return EXIT_FAILURE;
  }

  fclose(file);

  // Rotate the matrix
  rotateMatrix(matrix, rotated);

  // Print the rotated matrix
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      putchar(rotated[i][j]);
    }
    putchar('\n');
  }

  return EXIT_SUCCESS;
}

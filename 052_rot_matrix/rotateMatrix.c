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

  for (int i = 0; i < MATRIX_SIZE; i++) {
    if (fgets(line, sizeof(line), file) == NULL) {
      fprintf(stderr, "Error reading line %d\n", i + 1);
      fclose(file);
      return EXIT_FAILURE;
    }
    if (strchr(line, '\n') == NULL || strlen(line) != MATRIX_SIZE + 1) {
      fprintf(stderr, "Invalid line length at line %d\n", i + 1);
      fclose(file);
      return EXIT_FAILURE;
    }
    strncpy(matrix[i], line, MATRIX_SIZE);
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

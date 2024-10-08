#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "counts.h"
#include "kv.h"
#include "outname.h"
counts_t * countFile(const char * filename, kvarray_t * kvPairs);

void handleError(const char * msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

FILE * safeOpenFile(const char * filename, const char * mode) {
  FILE * f = fopen(filename, mode);
  if (f == NULL) {
    handleError("Cannot open file");
  }
  return f;
}

void safeCloseFile(FILE * f) {
  if (fclose(f) != 0) {
    handleError("Cannot close file");
  }
}

void processFile(const char * filename, kvarray_t * kvPairs) {
  counts_t * c = countFile(filename, kvPairs);
  char * outName = computeOutputFileName(filename);

  FILE * f = safeOpenFile(outName, "w");
  printCounts(c, f);
  safeCloseFile(f);

  freeCounts(c);
  free(outName);
}

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  FILE * f = safeOpenFile(filename, "r");
  char * line = NULL;
  size_t sz = 0;
  counts_t * c = createCounts();

  while (getline(&line, &sz, f) > 0) {
    if (strchr(line, '\n') != NULL) {
      *strchr(line, '\n') = '\0';
    }
    addCount(c, lookupValue(kvPairs, line));
    free(line);
    line = NULL;
  }

  free(line);
  safeCloseFile(f);
  return c;
}

int main(int argc, char ** argv) {
  if (argc <= 2) {
    handleError("Not enough arguments");
  }

  kvarray_t * kv = readKVs(argv[1]);

  for (int i = 2; i < argc; i++) {
    processFile(argv[i], kv);
  }

  freeKVs(kv);
  return EXIT_SUCCESS;
}

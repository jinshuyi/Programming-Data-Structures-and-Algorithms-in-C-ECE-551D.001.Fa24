#include "kv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

kvarray_t * readKVs(const char * fname) {
  kvarray_t * results = malloc(sizeof(*results));
  if (results == NULL) {
    return NULL;
  }
  results->pairs = malloc(sizeof(*results->pairs));
  if (results->pairs == NULL) {
    free(results);
    return NULL;
  }
  results->len = 0;
  char * line = NULL;
  size_t sz = 0;
  FILE * f = fopen(fname, "r");
  if (f == NULL) {
    free(results->pairs);
    free(results);
    return NULL;
  }
  while (getline(&line, &sz, f) >= 0) {
    results->len++;
    results->pairs = realloc(results->pairs, results->len * sizeof(*results->pairs));
    char * equal = strchr(line, '=');
    if (equal != NULL) {
      size_t key_len = equal - line;
      results->pairs[results->len - 1].key = strndup(line, key_len);
      char * next = strchr(equal + 1, '\n');
      if (next != NULL) {
        size_t value_len = next - equal - 1;
        results->pairs[results->len - 1].value = strndup(equal + 1, value_len);
      }
      else {
        results->pairs[results->len - 1].value = strdup(equal + 1);
      }
    }
    free(line);
    line = NULL;
  }
  free(line);
  fclose(f);
  return results;
}

void freeKVs(kvarray_t * pairs) {
  for (size_t i = 0; i < pairs->len; i++) {
    free(pairs->pairs[i].key);
    free(pairs->pairs[i].value);
  }
  free(pairs->pairs);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  for (size_t i = 0; i < pairs->len; i++) {
    printf("key = '%s' value = '%s'\n", pairs->pairs[i].key, pairs->pairs[i].value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  for (size_t i = 0; i < pairs->len; i++) {
    if (strcmp(key, pairs->pairs[i].key) == 0) {
      return pairs->pairs[i].value;
    }
  }
  return NULL;
}

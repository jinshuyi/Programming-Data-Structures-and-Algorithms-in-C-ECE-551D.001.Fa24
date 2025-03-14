#include "counts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
counts_t * createCounts(void) {
  counts_t * counts = malloc(sizeof(*counts));
  if (counts == NULL) {
    return NULL;
  }
  counts->words = NULL;
  counts->unknown = 0;
  counts->len = 0;
  return counts;
}

void addCount(counts_t * c, const char * name) {
  if (name == NULL) {
    c->unknown++;
    return;
  }

  for (size_t i = 0; i < c->len; i++) {
    if (strcmp(c->words[i].word, name) == 0) {
      c->words[i].times++;
      return;
    }
  }

  c->len++;
  c->words = realloc(c->words, c->len * sizeof(*c->words));
  if (c->words == NULL) {
    return;
  }

  one_count_t newCount;
  newCount.times = 1;
  newCount.word = strdup(name);
  if (newCount.word == NULL) {
    return;
  }

  c->words[c->len - 1] = newCount;
}

void printCounts(counts_t * c, FILE * outFile) {
  for (size_t i = 0; i < c->len; i++) {
    fprintf(outFile, "%s: %d\n", c->words[i].word, c->words[i].times);
  }
  if (c->unknown > 0) {
    fprintf(outFile, "<unknown> : %d\n", c->unknown);
  }
}

void freeCounts(counts_t * c) {
  for (size_t i = 0; i < c->len; i++) {
    free(c->words[i].word);
  }
  free(c->words);
  free(c);
}

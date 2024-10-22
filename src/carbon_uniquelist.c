// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>

#if _POSIX_C_SOURCE < 199309L
#warning Using custom implementation of `strdup`. If wanted to use the stdlib one, change to a different C standard

// NOTE: same implementation as in musl (https://git.musl-libc.org/cgit/musl/tree/src/string/strdup.c)
static char *strdup(const char *s) {
  size_t len = strlen(s);
  char *data = malloc(len + 1);
  if (!data) return 0;
  return memcpy(data, s, len + 1);
}
#endif

UniqueList carbon_uniquelist_create(void) {
  UniqueList ul = {
    .size = 0,
    .capacity = 1
  };
  size_t size = ul.capacity * sizeof(char *);
  ul.items = malloc(size);
  if (!ul.items) {
    CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_uniquelist_create :: failed to allocate memory (%zuB)" CARBON_COLOR_RESET "\n", size);
    exit(1);
  }
  return ul;
}

void carbon_uniquelist_destroy(UniqueList *ul) {
  for (size_t i = 0; i < ul->size; ++i) {
    free(ul->items[i]);
  }
  free(ul->items);
  *ul = (UniqueList) {0};
  ul = 0;
}

void carbon_uniquelist_push(UniqueList *ul, const char *s) {
  if (carbon_uniquelist_contains(ul, s)) return;
  if (ul->size == ul->capacity) {
    ul->capacity *= 2;
    char **prev_p = ul->items;
    size_t size = ul->capacity * sizeof(char *);
    ul->items = realloc(ul->items, size);
    if (!ul->items) {
      CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_uniquelist_push :: failed to reallocate memory (%zuB)" CARBON_COLOR_RESET "\n", size);
      free(prev_p);
      exit(1);
    }
  }
  ul->items[ul->size] = strdup(s);
  if (!ul->items[ul->size]) {
    CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_uniquelist_push :: failed to duplicate string" CARBON_COLOR_RESET "\n");
    exit(1);
  }
  ++ul->size;
}

static int find_idx(UniqueList *ul, const char *s) {
  for (size_t i = 0; i < ul->size; ++i) {
    if (!strcmp(ul->items[i], s)) return i;
  }
  return -1;
}

void carbon_uniquelist_pop(UniqueList *ul, const char *s) {
  int idx = find_idx(ul, s);
  if (idx == -1) {
    CARBON_INFO("[WARNING]: carbon_uniquelist_pop :: string not present in list. Skipping...");
    return;
  }
  free(ul->items[idx]);
  for (size_t i = idx; i < ul->size - 1; ++i) {
    ul->items[i] = ul->items[i + 1];
  }
  --ul->size;
  if (ul->size > 0 && ul->size < ul->capacity / 4) {
    ul->capacity /= 2;
    char **prev_p = ul->items;
    size_t size = ul->capacity * sizeof(char *);
    ul->items = realloc(ul->items, size);
    if (!ul->items && ul->size > 0) {
      CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_uniquelist_push :: failed to reallocate memory (%zuB)" CARBON_COLOR_RESET "\n", size);
      free(prev_p);
      exit(1);
    }
  }
}

unsigned char carbon_uniquelist_contains(UniqueList *ul, const char *s) {
  for (size_t i = 0; i < ul->size; ++i) {
    if (!strcmp(ul->items[i], s)) return 1;
  }
  return 0;
}

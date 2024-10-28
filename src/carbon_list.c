// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

CBN_List carbon_list_create(u8 unique) {
  CBN_List ul = {
    .size = 0,
    .capacity = 1,
    .unique = unique
  };
  usz size = ul.capacity * sizeof(char *);
  ul.items = CARBON_MALLOC(size);
  if (!ul.items) {
    CARBON_ERROR("carbon_list_create :: failed to allocate memory (%zuB)", size);
    exit(1);
  }
  return ul;
}

void carbon_list_destroy(CBN_List *ul) {
  for (usz i = 0; i < ul->size; ++i) {
    CARBON_FREE(ul->items[i]);
  }
  CARBON_FREE(ul->items);
  *ul = (CBN_List) {0};
  ul = 0;
}

void carbon_list_push(CBN_List *ul, const char *s) {
  if (ul->unique && carbon_list_contains(ul, s)) return;
  if (ul->size == ul->capacity) {
    ul->capacity *= 2;
    char **prev_p = ul->items;
    usz size = ul->capacity * sizeof(char *);
    ul->items = CARBON_REALLOC(ul->items, size);
    if (!ul->items) {
      CARBON_ERROR("carbon_list_push :: failed to reallocate memory (%zuB)", size);
      CARBON_FREE(prev_p);
      exit(1);
    }
  }
  ul->items[ul->size] = carbon_string_dup(s);
  if (!ul->items[ul->size]) {
    CARBON_ERROR("carbon_list_push :: failed to duplicate string");
    exit(1);
  }
  ++ul->size;
}

static int find_idx(CBN_List *ul, const char *s) {
  for (usz i = 0; i < ul->size; ++i) {
    if (!carbon_string_cmp(ul->items[i], s)) return i;
  }
  return -1;
}

void carbon_list_pop(CBN_List *ul, const char *s) {
  int idx = find_idx(ul, s);
  if (idx == -1) {
    CARBON_WARNING("carbon_list_pop :: string `%s` not present in list", s);
    return;
  }
  CARBON_FREE(ul->items[idx]);
  for (usz i = idx; i < ul->size - 1; ++i) {
    ul->items[i] = ul->items[i + 1];
  }
  --ul->size;
  if (ul->size > 0 && ul->size < ul->capacity / 4) {
    ul->capacity /= 2;
    char **prev_p = ul->items;
    usz size = ul->capacity * sizeof(char *);
    ul->items = CARBON_REALLOC(ul->items, size);
    if (!ul->items && ul->size > 0) {
      CARBON_ERROR("carbon_list_push :: failed to reallocate memory (%zuB)", size);
      CARBON_FREE(prev_p);
      exit(1);
    }
  }
}

u8 carbon_list_contains(CBN_List *ul, const char *s) {
  for (usz i = 0; i < ul->size; ++i) {
    if (!carbon_string_cmp(ul->items[i], s)) return true;
  }
  return false;
}

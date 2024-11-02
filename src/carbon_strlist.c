// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

static i32 carbon_strlist__find_idx(CBN_StrList *sl, const char *s) {
  for (usz i = 0; i < sl->size; ++i) {
    if (!carbon_string_cmp(sl->items[i], s)) return i;
  }
  return -1;
}

CBN_StrList carbon_strlist_create(u8 unique) {
  CBN_StrList sl = {
    .size = 0,
    .capacity = 1,
    .unique = unique
  };
  usz size = sl.capacity * sizeof(char *);
  sl.items = CARBON_MALLOC(size);
  if (!sl.items) {
    CARBON_ERROR("carbon_strlist_create :: failed to allocate memory (%zuB)", size);
    return (CBN_StrList) {0};
  }
  return sl;
}

void carbon_strlist_destroy(CBN_StrList *sl) {
  if (!sl) {
    CARBON_WARNING("carbon_strlist_destroy :: `sl` is not a valid pointer, skipping destruction");
    return;
  }
  for (usz i = 0; i < sl->size; ++i) {
    CARBON_FREE(sl->items[i]);
  }
  CARBON_FREE(sl->items);
  *sl = (CBN_StrList) {0};
  sl = 0;
}

void carbon_strlist_push(CBN_StrList *sl, const char *s) {
  if (!sl || !s) {
    CARBON_ERROR("carbon_strlist_push :: `sl` and `s` must be valid pointers");
    return;
  }
  if (sl->unique && carbon_strlist_contains(sl, s)) return;
  if (sl->size == sl->capacity) {
    sl->capacity *= 2;
    char **prev_p = sl->items;
    usz size = sl->capacity * sizeof(char *);
    sl->items = CARBON_REALLOC(sl->items, size);
    if (!sl->items) {
      CARBON_ERROR("carbon_strlist_push :: failed to reallocate memory (%zuB)", size);
      CARBON_FREE(prev_p);
      return;
    }
  }
  sl->items[sl->size] = carbon_string_dup(s);
  ++sl->size;
}

void carbon_strlist_pop(CBN_StrList *sl, const char *s) {
  if (!sl || !s) {
    CARBON_ERROR("carbon_strlist_pop :: `sl` and `s` must be valid pointers");
    return;
  }
  i32 idx = carbon_strlist__find_idx(sl, s);
  if (idx == -1) {
    CARBON_WARNING("carbon_strlist_pop :: string `%s` not present in list", s);
    return;
  }
  CARBON_FREE(sl->items[idx]);
  for (usz i = idx; i < sl->size - 1; ++i) {
    sl->items[i] = sl->items[i + 1];
  }
  --sl->size;
  if (sl->size > 0 && sl->size < sl->capacity / 4) {
    sl->capacity /= 2;
    char **prev_p = sl->items;
    usz size = sl->capacity * sizeof(char *);
    sl->items = CARBON_REALLOC(sl->items, size);
    if (!sl->items && sl->size > 0) {
      CARBON_ERROR("carbon_strlist_pop :: failed to reallocate memory (%zuB)", size);
      CARBON_FREE(prev_p);
      return;
    }
  }
}

u8 carbon_strlist_contains(CBN_StrList *sl, const char *s) {
  if (!sl || !s) {
    CARBON_ERROR("carbon_strlist_contains :: `sl` and `s` must be valid pointers");
    return false;
  }
  for (usz i = 0; i < sl->size; ++i) {
    if (!carbon_string_cmp(sl->items[i], s)) return true;
  }
  return false;
}

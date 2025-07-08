// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

static i32 carbon_strlist__find_idx(CBN_StrList *sl, const char *s) {
  for (usz i = 0; i < sl->size; ++i) {
    if (!carbon_string_cmp(sl->items[i], s)) return i;
  }
  return -1;
}

CBN_StrList carbon_strlist_create(u8 unique) {
  char **ptr = (char **) CBN_MALLOC(sizeof(char *));
  CBN_ASSERT(ptr && "failed to allocate memory");
  return (CBN_StrList) {
    .items = ptr,
    .size = 0,
    .capacity = 1,
    .unique = unique
  };
}

CBN_StrList carbon_strlist_from_splitted_cstr(const char *s, const char *delim) {
  CBN_StrList sl = carbon_strlist_create(false);
  char *s_copy = carbon_string_dup(s);
  usz delim_len = carbon_string_len(delim);
  char *start = s_copy;
  char *found;
  while ((found = strstr(start, delim))) {
    *found = 0;
    carbon_strlist_push(&sl, start);
    start = found + delim_len;
  }
  if (*start) carbon_strlist_push(&sl, start);
  CBN_FREE(s_copy);
  return sl;
}

void carbon_strlist_destroy(CBN_StrList *sl) {
  if (!sl) {
    carbon_log_warn("`sl` is not a valid pointer, skipping destruction");
    return;
  }
  for (usz i = 0; i < sl->size; ++i) {
    CBN_FREE(sl->items[i]);
  }
  CBN_FREE(sl->items);
  carbon_memory_set(sl, 0, sizeof(*sl));
}

void carbon_strlist_push(CBN_StrList *sl, const char *s) {
  if (!sl || !s) {
    carbon_log_error("`sl` and `s` must be valid pointers");
    return;
  }
  if (sl->unique && carbon_strlist_contains(sl, s)) return;
  if (sl->size == sl->capacity) {
    sl->capacity *= 2;
    char **prev_p = sl->items;
    usz size = sl->capacity * sizeof(char *);
    sl->items = (char **) CBN_REALLOC(sl->items, size);
    if (!sl->items) {
      carbon_log_error("failed to reallocate memory (%zuB)", size);
      CBN_FREE(prev_p);
      return;
    }
  }
  sl->items[sl->size] = carbon_string_dup(s);
  ++sl->size;
}

void carbon_strlist_pop(CBN_StrList *sl, const char *s) {
  if (!sl || !s) {
    carbon_log_error("`sl` and `s` must be valid pointers");
    return;
  }
  i32 idx = carbon_strlist__find_idx(sl, s);
  if (idx == -1) {
    carbon_log_warn("string `%s` not present in list", s);
    return;
  }
  CBN_FREE(sl->items[idx]);
  for (usz i = idx; i < sl->size - 1; ++i) {
    sl->items[i] = sl->items[i + 1];
  }
  --sl->size;
  if (sl->size > 0 && sl->size < sl->capacity / 4) {
    sl->capacity /= 2;
    char **prev_p = sl->items;
    usz size = sl->capacity * sizeof(char *);
    sl->items = (char **) CBN_REALLOC(sl->items, size);
    if (!sl->items && sl->size > 0) {
      carbon_log_error("failed to reallocate memory (%zuB)", size);
      CBN_FREE(prev_p);
      return;
    }
  }
}

u8 carbon_strlist_contains(CBN_StrList *sl, const char *s) {
  if (!sl || !s) {
    carbon_log_error("`sl` and `s` must be valid pointers");
    return false;
  }
  for (usz i = 0; i < sl->size; ++i) {
    if (!carbon_string_cmp(sl->items[i], s)) return true;
  }
  return false;
}

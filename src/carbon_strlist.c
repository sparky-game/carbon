// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

static i32 carbon_strlist__find_idx(CBN_StrList *sl, const char *s) {
  for (usz i = 0; i < sl->size; ++i) {
    if (!carbon_string_cmp(sl->items[i], s)) return i;
  }
  return -1;
}

CBN_StrList carbon_strlist_create(u8 unique) {
  return (CBN_StrList) {
    .items = (char **) carbon_memory_alloc(sizeof(char *)),
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
  carbon_memory_free(s_copy);
  return sl;
}

void carbon_strlist_destroy(CBN_StrList *sl) {
  if (!sl) {
    CBN_WARN("`sl` is not a valid pointer, skipping destruction");
    return;
  }
  for (usz i = 0; i < sl->size; ++i) {
    carbon_memory_free(sl->items[i]);
  }
  carbon_memory_free(sl->items);
  carbon_memory_set(sl, 0, sizeof(*sl));
}

void carbon_strlist_push(CBN_StrList *sl, const char *s) {
  if (!sl || !s) {
    CBN_ERROR("`sl` and `s` must be valid pointers");
    return;
  }
  if (sl->unique && carbon_strlist_contains(sl, s)) return;
  if (sl->size == sl->capacity) {
    sl->capacity *= 2;
    sl->items = (char **) carbon_memory_realloc(sl->items, sl->capacity * sizeof(char *));
  }
  sl->items[sl->size] = carbon_string_dup(s);
  ++sl->size;
}

void carbon_strlist_pop(CBN_StrList *sl, const char *s) {
  if (!sl || !s) {
    CBN_ERROR("`sl` and `s` must be valid pointers");
    return;
  }
  i32 idx = carbon_strlist__find_idx(sl, s);
  if (idx == -1) {
    CBN_WARN("string `%s` not present in list", s);
    return;
  }
  carbon_memory_free(sl->items[idx]);
  for (usz i = idx; i < sl->size - 1; ++i) {
    sl->items[i] = sl->items[i + 1];
  }
  --sl->size;
  if (sl->size > 0 && sl->size < sl->capacity / 4) {
    sl->capacity /= 2;
    sl->items = (char **) carbon_memory_realloc(sl->items, sl->capacity * sizeof(char *));
  }
}

u8 carbon_strlist_contains(CBN_StrList *sl, const char *s) {
  if (!sl || !s) {
    CBN_ERROR("`sl` and `s` must be valid pointers");
    return false;
  }
  for (usz i = 0; i < sl->size; ++i) {
    if (!carbon_string_cmp(sl->items[i], s)) return true;
  }
  return false;
}

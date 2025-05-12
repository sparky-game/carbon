// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

CBN_List carbon_list_create(usz stride) {
  CBN_List l = {
    .items = CARBON_MALLOC(stride),
    .capacity = 1,
    .stride = stride,
    .size = 0
  };
  if (!l.items) {
    carbon_log_error("failed to allocate memory (%zuB)", stride);
    memset(&l, 0, sizeof(CBN_List));
  }
  return l;
}

void carbon_list_destroy(CBN_List *l) {
  if (!l) {
    carbon_log_warn("`l` is not a valid pointer, skipping destruction");
    return;
  }
  if (l->items) CARBON_FREE(l->items);
  memset(l, 0, sizeof(CBN_List));
  l = 0;
}

void carbon_list_push(CBN_List *l, void *value) {
  if (!l || !value) {
    carbon_log_error("`l` and `value` must be valid pointers");
    return;
  }
  if (l->size == l->capacity) {
    l->capacity *= 2;
    void *prev_p = l->items;
    usz size = l->capacity * l->stride;
    l->items = CARBON_REALLOC(l->items, size);
    if (!l->items) {
      carbon_log_error("failed to reallocate memory (%zuB)", size);
      CARBON_FREE(prev_p);
      return;
    }
  }
  carbon_memory_copy((void *) ((u64) l->items + (l->size * l->stride)), value, l->stride);
  ++l->size;
}

void carbon_list_pop(CBN_List *l, void *out_value) {
  if (!l || !out_value) {
    carbon_log_error("`l` and `out_value` must be valid pointers");
    return;
  }
  if (!l->size) {
    carbon_log_warn("list is empty");
    return;
  }
  carbon_memory_copy(out_value, (void *) ((u64) l->items + ((l->size - 1) * l->stride)), l->stride);
  --l->size;
  if (l->size > 0 && l->size < l->capacity / 4) {
    l->capacity /= 2;
    void *prev_p = l->items;
    usz size = l->capacity * l->stride;
    l->items = CARBON_REALLOC(l->items, size);
    if (!l->items && l->size > 0) {
      carbon_log_error("failed to reallocate memory (%zuB)", size);
      CARBON_FREE(prev_p);
      return;
    }
  }
}

isz carbon_list_find(const CBN_List *l, const void *value) {
  if (!l || !value) {
    carbon_log_error("`l` and `value` must be valid pointers");
    return -1;
  }
  for (usz i = 0; i < l->size; ++i) {
    void *curr = (void *) ((u64) l->items + (i * l->stride));
    if (!memcmp(curr, value, l->stride)) return i;
  }
  return -1;
}

void carbon_list_remove(CBN_List *l, usz idx) {
  if (!l) {
    carbon_log_error("`l` must be a valid pointer");
    return;
  }
  if (idx >= l->size) {
    carbon_log_error("idx out of bounds (size: %$, idx: %$)", $(l->size), $(idx));
    return;
  }
  if (idx < l->size - 1) {
    void *dst = (void *) ((u64) l->items + (idx * l->stride));
    void *src = (void *) ((u64) l->items + ((idx + 1) * l->stride));
    memmove(dst, src, (l->size - idx - 1) * l->stride);
  }
  --l->size;
  if (l->size > 0 && l->size < l->capacity / 4) {
    l->capacity /= 2;
    void *prev_p = l->items;
    usz size = l->capacity * l->stride;
    l->items = CARBON_REALLOC(l->items, size);
    if (!l->items && l->size > 0) {
      carbon_log_error("failed to reallocate memory (%zuB)", size);
      CARBON_FREE(prev_p);
      return;
    }
  }
}

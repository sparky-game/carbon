// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_List carbon_list_create(usz stride) {
  void *ptr = CBN_MALLOC(stride);
  CBN_ASSERT(ptr && "failed to allocate memory");
  return (CBN_List) {
    .items = ptr,
    .capacity = 1,
    .stride = stride,
    .size = 0
  };
}

void carbon_list_destroy(CBN_List *l) {
  if (!l) {
    CBN_WARN("`l` is not a valid pointer, skipping destruction");
    return;
  }
  if (l->items) CBN_FREE(l->items);
  carbon_memory_set(l, 0, sizeof(*l));
}

void carbon_list_push(CBN_List *l, void *value) {
  if (!l || !value) {
    CBN_ERROR("`l` and `value` must be valid pointers");
    return;
  }
  if (l->size == l->capacity) {
    l->capacity *= 2;
    void *prev_p = l->items;
    usz size = l->capacity * l->stride;
    l->items = CBN_REALLOC(l->items, size);
    if (!l->items) {
      CBN_ERROR("failed to reallocate memory (%zuB)", size);
      CBN_FREE(prev_p);
      return;
    }
  }
  carbon_memory_copy((void *) ((u64) l->items + (l->size * l->stride)), value, l->stride);
  ++l->size;
}

void carbon_list_pop(CBN_List *l, void *out_value) {
  if (!l || !out_value) {
    CBN_ERROR("`l` and `out_value` must be valid pointers");
    return;
  }
  if (!l->size) {
    CBN_WARN("list is empty");
    return;
  }
  carbon_memory_copy(out_value, (void *) ((u64) l->items + ((l->size - 1) * l->stride)), l->stride);
  --l->size;
  if (l->size > 0 && l->size < l->capacity / 4) {
    l->capacity /= 2;
    void *prev_p = l->items;
    usz size = l->capacity * l->stride;
    l->items = CBN_REALLOC(l->items, size);
    if (!l->items && l->size > 0) {
      CBN_ERROR("failed to reallocate memory (%zuB)", size);
      CBN_FREE(prev_p);
      return;
    }
  }
}

isz carbon_list_find(const CBN_List *l, const void *value) {
  if (!l || !value) {
    CBN_ERROR("`l` and `value` must be valid pointers");
    return -1;
  }
  for (usz i = 0; i < l->size; ++i) {
    void *curr = (void *) ((u64) l->items + (i * l->stride));
    if (!carbon_memory_cmp(curr, value, l->stride)) return i;
  }
  return -1;
}

void carbon_list_remove(CBN_List *l, usz idx) {
  if (!l) {
    CBN_ERROR("`l` must be a valid pointer");
    return;
  }
  if (idx >= l->size) {
    CBN_ERROR("idx out of bounds (size: %$, idx: %$)", $(l->size), $(idx));
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
    l->items = CBN_REALLOC(l->items, size);
    if (!l->items && l->size > 0) {
      CBN_ERROR("failed to reallocate memory (%zuB)", size);
      CBN_FREE(prev_p);
      return;
    }
  }
}

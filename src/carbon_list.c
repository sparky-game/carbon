// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_LIST__RESIZE_FACTOR 2
#define CARBON_LIST__FIRST_ALLOC_CAPACITY 2

CBN_List carbon_list_create(usz stride) {
  return (CBN_List) {
    .items = 0,
    .capacity = 0,
    .stride = stride,
    .size = 0
  };
}

void carbon_list_destroy(CBN_List *l) {
  if (!l) {
    CBN_WARN("`l` is not a valid pointer, skipping destruction");
    return;
  }
  carbon_memory_free(l->items);
  carbon_memory_set(l, 0, sizeof(*l));
}

void carbon_list_push(CBN_List *l, void *value) {
  if (!l || !value) {
    CBN_ERROR("`l` and `value` must be valid pointers");
    return;
  }
  if (l->size == l->capacity) {
    if (!l->capacity) l->capacity = CARBON_LIST__FIRST_ALLOC_CAPACITY;
    else l->capacity *= CARBON_LIST__RESIZE_FACTOR;
    l->items = carbon_memory_realloc(l->items, l->capacity * l->stride);
  }
  carbon_memory_copy((void *)((u64)l->items + (l->size * l->stride)), value, l->stride);
  ++l->size;
}

void carbon_list_pop_front(CBN_List *l, void *out_value) {
  if (!l || !out_value) {
    CBN_ERROR("`l` and `out_value` must be valid pointers");
    return;
  }
  if (!l->size) {
    CBN_WARN("list is empty");
    return;
  }
  carbon_memory_copy(out_value, l->items, l->stride);
  if (l->size > 1) {
    void *dst = l->items;
    void *src = (void *)((u64)l->items + l->stride);
    memmove(dst, src, (l->size - 1) * l->stride);
  }
  --l->size;
}

void carbon_list_pop_back(CBN_List *l, void *out_value) {
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
  void *dst = (void *)((u64)l->items + (idx * l->stride));
  void *src = (void *)((u64)l->items + ((idx + 1) * l->stride));
  memmove(dst, src, (l->size - idx - 1) * l->stride);
  --l->size;
}

void carbon_list_shrink_to_fit(CBN_List *l) {
  if (l->size > 0 && l->size < l->capacity / 4) {
    l->capacity = l->size;
    l->items = carbon_memory_realloc(l->items, l->capacity * l->stride);
  }
}

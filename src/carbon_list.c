// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

CBN_List carbon_list_create(usz stride) {
  CBN_List l = {
    .capacity = 1,
    .stride = stride,
    .size = 0
  };
  usz size = l.capacity * l.stride;
  l.items = CARBON_MALLOC(size);
  if (!l.items) {
    CARBON_ERROR("carbon_list_create :: failed to allocate memory (%zuB)", size);
    return (CBN_List) {0};
  }
  return l;
}

void carbon_list_destroy(CBN_List *l) {
  if (!l) {
    CARBON_WARNING("carbon_list_destroy :: `l` is not a valid pointer, skipping destruction");
    return;
  }
  CARBON_FREE(l->items);
  *l = (CBN_List) {0};
  l = 0;
}

void carbon_list_push(CBN_List *l, void *value) {
  if (!l || !value) {
    CARBON_ERROR("carbon_list_push :: `l` and `value` must be valid pointers");
    return;
  }
  if (l->size == l->capacity) {
    l->capacity *= 2;
    void *prev_p = l->items;
    usz size = l->capacity * l->stride;
    l->items = CARBON_REALLOC(l->items, size);
    if (!l->items) {
      CARBON_ERROR("carbon_list_push :: failed to reallocate memory (%zuB)", size);
      CARBON_FREE(prev_p);
      return;
    }
  }
  memcpy((void *) ((u64) l->items + (l->size * l->stride)), value, l->stride);
  ++l->size;
}

void carbon_list_pop(CBN_List *l, void *out_value) {
  if (!l || !out_value) {
    CARBON_ERROR("carbon_list_pop :: `l` and `out_value` must be valid pointers");
    return;
  }
  if (!l->size) {
    CARBON_WARNING("carbon_list_pop :: list is empty");
    return;
  }
  memcpy(out_value, (void *) ((u64) l->items + ((l->size - 1) * l->stride)), l->stride);
  --l->size;
  if (l->size > 0 && l->size < l->capacity / 4) {
    l->capacity /= 2;
    void *prev_p = l->items;
    usz size = l->capacity * l->stride;
    l->items = CARBON_REALLOC(l->items, size);
    if (!l->items && l->size > 0) {
      CARBON_ERROR("carbon_list_pop :: failed to reallocate memory (%zuB)", size);
      CARBON_FREE(prev_p);
      return;
    }
  }
}

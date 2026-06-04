// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_LIST__RESIZE_FACTOR 1.5
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
  if (!l) return;
  carbon_memory_free(l->items);
  carbon_memory_set(l, 0, sizeof(*l));
}

void carbon_list_clear(CBN_List *l) {
  if (!l) return;
  l->size = 0;
}

CBNINL usz carbon_list__next_capacity(usz curr, usz min) {
  if (!min) return 0;
  usz new = curr ?: CARBON_LIST__FIRST_ALLOC_CAPACITY;
  while (new < min) new *= CARBON_LIST__RESIZE_FACTOR;
  return new;
}

void carbon_list_reserve(CBN_List *l, usz min) {
  if (!l || min <= l->capacity) return;
  l->capacity = carbon_list__next_capacity(l->capacity, min);
  l->items = carbon_memory_realloc(l->items, l->capacity * l->stride);
}

void carbon_list_shrink_to_fit(CBN_List *l) {
  if (!l) return;
  if (0 < l->size && l->size < l->capacity/4) {
    l->capacity = l->size;
    l->items = carbon_memory_realloc(l->items, l->capacity * l->stride);
  }
}

void carbon_list_front(CBN_List *l, void *out_value) {
  if (!l || !out_value || !l->size) return;
  carbon_memory_copy(out_value, l->items, l->stride);
}

void carbon_list_back(CBN_List *l, void *out_value) {
  if (!l || !out_value || !l->size) return;
  carbon_memory_copy(out_value, l->items + (l->size - 1)*l->stride, l->stride);
}

void carbon_list_push(CBN_List *l, void *value) {
  if (!l || !value) return;
  carbon_list_reserve(l, l->size + 1);
  carbon_memory_copy(l->items + l->size*l->stride, value, l->stride);
  ++l->size;
}

void carbon_list_push_range(CBN_List *l, CBN_Span range) {
  if (!l || !range.data) return;
  carbon_list_reserve(l, l->size + range.size);
  carbon_memory_copy(l->items + l->size*l->stride, range.data, range.size*l->stride);
  l->size += range.size;
}

void carbon_list_pop_front(CBN_List *l, void *out_value) {
  carbon_list_front(l, out_value);
  if (!l || !l->size || !out_value) return;
  if (l->size > 1) {
    void *dst = l->items;
    void *src = l->items + l->stride;
    carbon_memory_copy(dst, src, (l->size - 1)*l->stride);
  }
  --l->size;
}

void carbon_list_pop_back(CBN_List *l, void *out_value) {
  carbon_list_back(l, out_value);
  if (!l || !l->size || !out_value) return;
  --l->size;
}

void carbon_list_assign(CBN_List *l, usz count, void *value) {
  if (!l || !value) return;
  carbon_list_clear(l);
  for (usz i = 0; i < count; ++i) {
    carbon_list_push(l, value);
  }
}

void carbon_list_assign_range(CBN_List *l, CBN_Span range) {
  if (!l || !range.data) return;
  carbon_list_clear(l);
  carbon_list_push_range(l, range);
}

isz carbon_list_find(const CBN_List *l, const void *value) {
  if (!l || !value) return -1;
  for (usz i = 0; i < l->size; ++i) {
    void *curr = l->items + i*l->stride;
    if (!carbon_memory_cmp(curr, value, l->stride)) return i;
  }
  return -1;
}

void carbon_list_remove(CBN_List *l, usz idx) {
  if (!l) return;
  CBN_ASSERT(idx < l->size && "List index out of bounds");
  if (idx < l->size - 1) {
    void *dst = l->items + idx*l->stride;
    void *src = l->items + (idx + 1)*l->stride;
    carbon_memory_copy(dst, src, (l->size - idx - 1)*l->stride);
  }
  --l->size;
}

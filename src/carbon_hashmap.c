// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

static u64 carbon_hashmap__hash(const char *key, u32 n) {
  u64 hash = 0;
  for (const u8 *p = (const u8 *) key; *p; ++p) {
    hash = (hash * 97) + *p;
  }
  return hash % n;
}

CBN_HashMap carbon_hashmap_create(usz capacity, usz stride) {
  CBN_HashMap hm = {
    .capacity = capacity,
    .stride = stride
  };
  usz size = hm.capacity * hm.stride;
  hm.items = CARBON_MALLOC(size);
  if (!hm.items) {
    CARBON_ERROR("carbon_hashmap_create :: failed to allocate memory (%zuB)", size);
    return (CBN_HashMap) {0};
  }
  return hm;
}

void carbon_hashmap_destroy(CBN_HashMap *hm) {
  if (!hm) {
    CARBON_WARNING("carbon_hashmap_destroy :: `hm` is not a valid pointer, skipping destruction");
    return;
  }
  CARBON_FREE(hm->items);
  *hm = (CBN_HashMap) {0};
  hm = 0;
}

void carbon_hashmap_set(CBN_HashMap *hm, const char *key, void *value) {
  if (!hm || !key || !value) {
    CARBON_ERROR("carbon_hashmap_set :: `hm`, `key` and `value` must be valid pointers");
    return;
  }
  memcpy((void *) ((u8 *) hm->items + (carbon_hashmap__hash(key, hm->capacity) * hm->stride)), value, hm->stride);
}

void carbon_hashmap_get(CBN_HashMap *hm, const char *key, void *out_value) {
  if (!hm || !key || !out_value) {
    CARBON_ERROR("carbon_hashmap_set :: `hm`, `key` and `out_value` must be valid pointers");
    return;
  }
  memcpy(out_value, (void *) ((u8 *) hm->items + (carbon_hashmap__hash(key, hm->capacity) * hm->stride)), hm->stride);
}
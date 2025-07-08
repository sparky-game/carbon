// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CARBON_INLINE u64 carbon_hashmap__hash(const char *key, u32 n) {
  u64 hash = 0;
  for (const u8 *p = (const u8 *) key; *p; ++p) {
    hash = (hash * 97) + *p;
  }
  return hash % n;
}

CBN_HashMap carbon_hashmap_create(usz capacity, usz stride) {
  void *ptr = CBN_MALLOC(capacity * stride);
  CBN_ASSERT(ptr && "failed to allocate memory");
  return (CBN_HashMap) {
    .items = ptr,
    .capacity = capacity,
    .stride = stride
  };
}

void carbon_hashmap_destroy(CBN_HashMap *hm) {
  if (!hm) {
    CBN_WARN("`hm` is not a valid pointer, skipping destruction");
    return;
  }
  CBN_FREE(hm->items);
  carbon_memory_set(hm, 0, sizeof(*hm));
}

void carbon_hashmap_set(CBN_HashMap *hm, const char *key, void *value) {
  if (!hm || !key || !value) {
    CBN_ERROR("`hm`, `key` and `value` must be valid pointers");
    return;
  }
  carbon_memory_copy((void *) ((u8 *) hm->items + (carbon_hashmap__hash(key, hm->capacity) * hm->stride)), value, hm->stride);
}

void carbon_hashmap_get(const CBN_HashMap *hm, const char *key, void *out_value) {
  if (!hm || !key || !out_value) {
    CBN_ERROR("`hm`, `key` and `out_value` must be valid pointers");
    return;
  }
  carbon_memory_copy(out_value, (void *) ((u8 *) hm->items + (carbon_hashmap__hash(key, hm->capacity) * hm->stride)), hm->stride);
}

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

CARBON_INLINE u64 carbon_hashmap__hash(const char *key, u32 n) {
  u64 hash = 0;
  for (const u8 *p = (const u8 *) key; *p; ++p) {
    hash = (hash * 97) + *p;
  }
  return hash % n;
}

CBN_HashMap carbon_hashmap_create(usz capacity, usz stride) {
  CBN_HashMap hm = {
    .items = CARBON_CALLOC(capacity, stride),
    .capacity = capacity,
    .stride = stride
  };
  if (!hm.items) {
    carbon_log_error("failed to allocate memory (%zuB)", capacity * stride);
    memset(&hm, 0, sizeof(CBN_HashMap));
  }
  return hm;
}

void carbon_hashmap_destroy(CBN_HashMap *hm) {
  if (!hm) {
    carbon_log_warn("`hm` is not a valid pointer, skipping destruction");
    return;
  }
  CARBON_FREE(hm->items);
  memset(hm, 0, sizeof(CBN_HashMap));
  hm = 0;
}

void carbon_hashmap_set(CBN_HashMap *hm, const char *key, void *value) {
  if (!hm || !key || !value) {
    carbon_log_error("`hm`, `key` and `value` must be valid pointers");
    return;
  }
  carbon_memory_copy((void *) ((u8 *) hm->items + (carbon_hashmap__hash(key, hm->capacity) * hm->stride)), value, hm->stride);
}

void carbon_hashmap_get(CBN_HashMap *hm, const char *key, void *out_value) {
  if (!hm || !key || !out_value) {
    carbon_log_error("`hm`, `key` and `out_value` must be valid pointers");
    return;
  }
  carbon_memory_copy(out_value, (void *) ((u8 *) hm->items + (carbon_hashmap__hash(key, hm->capacity) * hm->stride)), hm->stride);
}

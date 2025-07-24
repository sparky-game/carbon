// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_HashMap carbon_hashmap_create(usz capacity, usz stride) {
  CBN_HashMap_Node **buckets = (CBN_HashMap_Node **) CBN_MALLOC(capacity * sizeof(CBN_HashMap_Node *));
  CBN_ASSERT(buckets && "failed to allocate memory");
  for (usz i = 0; i < capacity; ++i) buckets[i] = 0;
  return (CBN_HashMap) {
    .buckets = buckets,
    .capacity = capacity,
    .stride = stride
  };
}

void carbon_hashmap_destroy(CBN_HashMap *hm) {
  if (!hm) {
    CBN_WARN("`hm` is not a valid pointer, skipping destruction");
    return;
  }
  for (usz i = 0; i < hm->capacity; ++i) {
    CBN_HashMap_Node *curr = hm->buckets[i];
    while (curr) {
      CBN_HashMap_Node *next = curr->next;
      CBN_FREE((void *) curr->key);
      CBN_FREE(curr);
      curr = next;
    }
  }
  CBN_FREE(hm->buckets);
  carbon_memory_set(hm, 0, sizeof(*hm));
}

void carbon_hashmap_set(CBN_HashMap *hm, const char *key, void *value) {
  if (!hm || !key || !value) {
    CBN_ERROR("`hm`, `key` and `value` must be valid pointers");
    return;
  }
  CBN_HashMap_Node **head = &hm->buckets[carbon_crypto_djb2(key) % hm->capacity];
  for (CBN_HashMap_Node *curr = *head; curr; curr = curr->next) {
    if (carbon_string_cmp(curr->key, key)) continue;
    carbon_memory_copy(curr->value, value, hm->stride);
    return;
  }
  CBN_HashMap_Node *new = (CBN_HashMap_Node *) CBN_MALLOC(sizeof(CBN_HashMap_Node) + hm->stride);
  CBN_ASSERT(new && "failed to allocate memory");
  new->key = carbon_string_dup(key);
  carbon_memory_copy(new->value, value, hm->stride);
  new->next = *head;
  *head = new;
}

void carbon_hashmap_get(const CBN_HashMap *hm, const char *key, void *out_value) {
  if (!hm || !key || !out_value) {
    CBN_ERROR("`hm`, `key` and `out_value` must be valid pointers");
    return;
  }
  for (CBN_HashMap_Node *curr = hm->buckets[carbon_crypto_djb2(key) % hm->capacity]; curr; curr = curr->next) {
    if (carbon_string_cmp(curr->key, key)) continue;
    carbon_memory_copy(out_value, curr->value, hm->stride);
    return;
  }
  CBN_ERROR("key (`%s`) not found", key);
}

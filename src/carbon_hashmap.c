// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_HASHMAP__RESIZE_FACTOR 2
#define CARBON_HASHMAP__INITIAL_CAPACITY 16

CBN_HashMap carbon_hashmap_create(usz stride) {
  return (CBN_HashMap) {
    .buckets = 0,
    .capacity = 0,
    .stride = stride,
    .size = 0
  };
}

void carbon_hashmap_destroy(CBN_HashMap *hm) {
  if (!hm || !hm->buckets) return;
  for (usz i = 0; i < hm->capacity; ++i) {
    CBN_HashMap_Node *curr = hm->buckets[i];
    while (curr) {
      CBN_HashMap_Node *next = curr->next;
      carbon_memory_free((void *)curr->key);
      carbon_memory_free(curr);
      curr = next;
    }
  }
  carbon_memory_free(hm->buckets);
  carbon_memory_set(hm, 0, sizeof(*hm));
}

CBNINL void carbon_hashmap__init(CBN_HashMap *hm) {
  hm->capacity = CARBON_HASHMAP__INITIAL_CAPACITY;
  hm->buckets = carbon_memory_alloc(hm->capacity * sizeof(CBN_HashMap_Node *));
  for (usz i = 0; i < hm->capacity; ++i) hm->buckets[i] = 0;
}

CBNINL void carbon_hashmap__resize(CBN_HashMap *hm, usz new_cap) {
  CBN_HashMap_Node **buckets = carbon_memory_alloc(new_cap * sizeof(CBN_HashMap_Node *));
  for (usz i = 0; i < new_cap; ++i) buckets[i] = 0;
  for (usz i = 0; i < hm->capacity; ++i) {
    CBN_HashMap_Node *curr = hm->buckets[i];
    while (curr) {
      CBN_HashMap_Node *next = curr->next;
      usz idx = carbon_crypto_djb2(curr->key) % new_cap;
      curr->next = buckets[idx];
      buckets[idx] = curr;
      curr = next;
    }
  }
  carbon_memory_free(hm->buckets);
  hm->buckets = buckets;
  hm->capacity = new_cap;
}

void carbon_hashmap_set(CBN_HashMap *hm, const char *key, void *value) {
  if (!hm || !key || !value) return;
  if (!hm->buckets) carbon_hashmap__init(hm);
  CBN_HashMap_Node **head = &hm->buckets[carbon_crypto_djb2(key) % hm->capacity];
  for (CBN_HashMap_Node *curr = *head; curr; curr = curr->next) {
    if (carbon_string_cmp(curr->key, key)) continue;
    carbon_memory_copy(curr->value, value, hm->stride);
    return;
  }
  if (4*(hm->size + 1) > 3*hm->capacity) {
    carbon_hashmap__resize(hm, hm->capacity * CARBON_HASHMAP__RESIZE_FACTOR);
    head = &hm->buckets[carbon_crypto_djb2(key) % hm->capacity];
  }
  CBN_HashMap_Node *new = carbon_memory_alloc(sizeof(CBN_HashMap_Node) + hm->stride);
  new->key = carbon_string_dup(key);
  carbon_memory_copy(new->value, value, hm->stride);
  new->next = *head;
  *head = new;
  ++hm->size;
}

bool carbon_hashmap_get(const CBN_HashMap *hm, const char *key, void *out_value) {
  if (!hm || !key || !out_value) return false;
  for (CBN_HashMap_Node *curr = hm->buckets[carbon_crypto_djb2(key) % hm->capacity]; curr; curr = curr->next) {
    if (carbon_string_cmp(curr->key, key)) continue;
    carbon_memory_copy(out_value, curr->value, hm->stride);
    return true;
  }
  return false;
}

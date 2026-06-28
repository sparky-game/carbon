// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_HASHMAP__RESIZE_FACTOR 2
#define CARBON_HASHMAP__INITIAL_CAPACITY 16
#define CARBON_HASHMAP__HASH_FUNC carbon_crypto_fnv1a

usz carbon_hashmap_hash_mem(const void *p, usz n) {
  return CARBON_HASHMAP__HASH_FUNC(carbon_span_from_buf((void *)p, n));
}

usz carbon_hashmap_hash_cstr(const void *p, usz n) {
  CARBON_UNUSED(n);
  return CARBON_HASHMAP__HASH_FUNC(carbon_strview_from_cstr(*(const char **)p));
}

bool carbon_hashmap_eq_mem(const void *a, const void *b, usz n) {
  return !carbon_memory_cmp(a, b, n);
}

bool carbon_hashmap_eq_cstr(const void *a, const void *b, usz n) {
  CARBON_UNUSED(n);
  return !carbon_string_cmp(*(const char **)a, *(const char **)b);
}

CBN_HashMap carbon_hashmap_create(usz k_stride, usz v_stride, CBN_HashMap_HashFunc hash, CBN_HashMap_EqFunc eq) {
  return (CBN_HashMap) {
    .buckets = 0,
    .capacity = 0,
    .k_stride = k_stride,
    .v_stride = v_stride,
    .size = 0,
    .hash_fn = hash,
    .eq_fn = eq
  };
}

void carbon_hashmap_destroy(CBN_HashMap *hm) {
  carbon_hashmap_clear(hm);
  carbon_memory_free(hm->buckets);
  carbon_memory_set(hm, 0, sizeof(*hm));
}

void carbon_hashmap_clear(CBN_HashMap *hm) {
  if (!hm || !hm->buckets) return;
  for (usz i = 0; i < hm->capacity; ++i) {
    CBN_HashMap_Node *curr = hm->buckets[i];
    while (curr) {
      CBN_HashMap_Node *next = curr->next;
      carbon_memory_free(curr);
      curr = next;
    }
    hm->buckets[i] = 0;
  }
  hm->size = 0;
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
      usz idx = hm->hash_fn(curr->kv, hm->k_stride) % new_cap;
      curr->next = buckets[idx];
      buckets[idx] = curr;
      curr = next;
    }
  }
  carbon_memory_free(hm->buckets);
  hm->buckets = buckets;
  hm->capacity = new_cap;
}

void *carbon_hashmap_set(CBN_HashMap *hm, void *key, void *value) {
  if (!hm || !key) return 0;
  if (!hm->buckets) carbon_hashmap__init(hm);
  CBN_HashMap_Node **head = &hm->buckets[hm->hash_fn(key, hm->k_stride) % hm->capacity];
  for (CBN_HashMap_Node *curr = *head; curr; curr = curr->next) {
    if (!hm->eq_fn(curr->kv, key, hm->k_stride)) continue;
    if (value) carbon_memory_copy(curr->kv + hm->k_stride, value, hm->v_stride);
    return curr->kv + hm->k_stride;
  }
  if (4*(hm->size + 1) > 3*hm->capacity) {// TODO: factor out this into a LOAD_FACTOR macro.
    carbon_hashmap__resize(hm, hm->capacity * CARBON_HASHMAP__RESIZE_FACTOR);
    head = &hm->buckets[hm->hash_fn(key, hm->k_stride) % hm->capacity];
  }
  CBN_HashMap_Node *new = carbon_memory_alloc(sizeof(CBN_HashMap_Node) + hm->k_stride + hm->v_stride);
  carbon_memory_copy(new->kv, key, hm->k_stride);
  if (value) carbon_memory_copy(new->kv + hm->k_stride, value, hm->v_stride);
  new->next = *head;
  *head = new;
  ++hm->size;
  return new->kv + hm->k_stride;
}

void *carbon_hashmap_get(const CBN_HashMap *hm, void *key) {
  if (!hm || !hm->buckets || !key) return 0;
  CBN_HashMap_Node *curr = hm->buckets[hm->hash_fn(key, hm->k_stride) % hm->capacity];
  while (curr) {
    if (hm->eq_fn(curr->kv, key, hm->k_stride)) return curr->kv + hm->k_stride;
    curr = curr->next;
  }
  return 0;
}

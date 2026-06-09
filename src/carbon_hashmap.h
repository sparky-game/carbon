/*
  $$=====================$$
  ||       HashMap       ||
  $$=====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents ...
 */
CBNDEF_T(CBN_HashMap_Node) {
  CBN_HashMap_Node *next;
  u8 kv[];
};

typedef usz (*CBN_HashMap_HashFunc)(const void *p, usz n);
CBNDEF usz carbon_hashmap_hash_mem(const void *p, usz n);
CBNDEF usz carbon_hashmap_hash_cstr(const void *p, usz n);

typedef bool (*CBN_HashMap_EqFunc)(const void *a, const void *b, usz n);
CBNDEF bool carbon_hashmap_eq_mem(const void *a, const void *b, usz n);
CBNDEF bool carbon_hashmap_eq_cstr(const void *a, const void *b, usz n);

/**
 * @brief Represents ...
 */
CBNDEF_TT2(CBN_HashMap) {
  CBN_HashMap_Node **buckets;
  usz capacity;
  usz k_stride, v_stride;
  usz size;
  CBN_HashMap_HashFunc hash_fn;
  CBN_HashMap_EqFunc eq_fn;
};
CBNDEF_TT2AKA(cbn, HashMap, CBN_HashMap);

/**
 */
CBNDEF CBN_HashMap carbon_hashmap_create(usz k_stride, usz v_stride, CBN_HashMap_HashFunc hash, CBN_HashMap_EqFunc eq);

/**
 */
CBNDEF void carbon_hashmap_destroy(CBN_HashMap *hm);

/**
 */
CBNDEF void carbon_hashmap_set(CBN_HashMap *hm, void *key, void *value);

/**
 */
CBNDEF void *carbon_hashmap_get(const CBN_HashMap *hm, void *key);

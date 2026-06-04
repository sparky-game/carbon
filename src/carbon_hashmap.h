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
  const char *key;
  CBN_HashMap_Node *next;
  u8 value[];
};

/**
 * @brief Represents ...
 */
CBNDEF_TT(CBN_HashMap) {
  CBN_HashMap_Node **buckets;
  usz capacity;
  usz stride;
  usz size;
};
CBNDEF_TTAKA(cbn, HashMap, CBN_HashMap);

CBNDEF CBN_HashMap carbon_hashmap_create(usz stride);

CBNDEF void carbon_hashmap_destroy(CBN_HashMap *hm);

CBNDEF void carbon_hashmap_set(CBN_HashMap *hm, const char *key, void *value);

CBNDEF bool carbon_hashmap_get(const CBN_HashMap *hm, const char *key, void *out_value);

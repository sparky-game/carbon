/*
  $$=====================$$
  ||       HashMap       ||
  $$=====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

typedef struct Node {
  const char *key;
  struct Node *next;
  u8 value[];
} CBN_HashMap_Node;

typedef struct {
  CBN_HashMap_Node **buckets;
  usz capacity;
  usz stride;
} CBN_HashMap;

CBNDEF CBN_HashMap carbon_hashmap_create(usz capacity, usz stride);

CBNDEF void carbon_hashmap_destroy(CBN_HashMap *hm);

CBNDEF void carbon_hashmap_set(CBN_HashMap *hm, const char *key, void *value);

CBNDEF u8 carbon_hashmap_get(const CBN_HashMap *hm, const char *key, void *out_value);

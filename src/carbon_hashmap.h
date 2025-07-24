/*
**  $$=====================$$
**  ||       HashMap       ||
**  $$=====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

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

CARBON_API CBN_HashMap carbon_hashmap_create(usz capacity, usz stride);

CARBON_API void carbon_hashmap_destroy(CBN_HashMap *hm);

CARBON_API void carbon_hashmap_set(CBN_HashMap *hm, const char *key, void *value);

CARBON_API void carbon_hashmap_get(const CBN_HashMap *hm, const char *key, void *out_value);

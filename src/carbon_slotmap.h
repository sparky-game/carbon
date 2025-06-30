/*
**  $$=====================$$
**  ||       SlotMap       ||
**  $$=====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

typedef struct {
  u64 id;
  u64 gen;
} CBN_SlotMap_Key;

typedef struct {
  usz stride;
  u64 size;
  u64 freelist;
  u64 generation;
  CBN_List indices;
  CBN_List data;
  CBN_List erase;
} CBN_SlotMap;

CARBON_API CBN_SlotMap carbon_slotmap_create(usz stride);

CARBON_API void carbon_slotmap_destroy(CBN_SlotMap *sm);

CARBON_API CBN_SlotMap_Key carbon_slotmap_push(CBN_SlotMap *sm, void *value);

CARBON_API u8 carbon_slotmap_remove(CBN_SlotMap *sm, const CBN_SlotMap_Key key);

CARBON_API u8 carbon_slotmap_lookup(const CBN_SlotMap *sm, const CBN_SlotMap_Key key, void *out_value);

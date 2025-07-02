// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CARBON_INLINE u8 carbon_slotmap__is_valid_key(const CBN_SlotMap *sm, const CBN_SlotMap_Key key) {
  if (carbon_list_at(CBN_SlotMap_Key, sm->indices, key.id).gen != key.gen) return false;
  return true;
}

CARBON_INLINE u64 carbon_slotmap__alloc(CBN_SlotMap *sm) {
  u64 slot_id = sm->freelist;
  u64 idx = slot_id + 1;
  carbon_list_push(&sm->indices, &idx);
  sm->freelist = carbon_list_at(CBN_SlotMap_Key, sm->indices, slot_id).id;
  CBN_SlotMap_Key *slot = &carbon_list_at_raw(CBN_SlotMap_Key, sm->indices, slot_id);
  slot->id = sm->size;
  slot->gen = sm->generation;
  ++sm->size;
  ++sm->generation;
  return slot_id;
}

CARBON_INLINE void carbon_slotmap__free(CBN_SlotMap *sm, const CBN_SlotMap_Key key) {
  CARBON_ASSERT(carbon_slotmap__is_valid_key(sm, key) && "Key is not valid");
  CBN_SlotMap_Key *slot = &carbon_list_at_raw(CBN_SlotMap_Key, sm->indices, key.id);
  u64 data_id = slot->id;
  slot->id = sm->freelist;
  slot->gen = sm->generation;
  sm->freelist = key.id;
  if (data_id != sm->size - 1) {
    carbon_memory_copy((void *) ((u64) sm->data.items + (data_id * sm->stride)), (void *) ((u64) sm->data.items + ((sm->size - 1) * sm->stride)), sm->stride);
    carbon_list_at_raw(u64, sm->erase, data_id) = carbon_list_at(u64, sm->erase, sm->size - 1);
    carbon_list_at_raw(CBN_SlotMap_Key, sm->indices, carbon_list_at_raw(u64, sm->erase, data_id)).id = data_id;
  }
  carbon_list_remove(&sm->data, sm->size - 1);
  --sm->size;
  ++sm->generation;
}

CBN_SlotMap carbon_slotmap_create(usz stride) {
  return (CBN_SlotMap) {
    .stride = stride,
    .size = 0,
    .freelist = 0,
    .generation = 0,
    .indices = carbon_list_create(sizeof(CBN_SlotMap_Key)),
    .data = carbon_list_create(stride),
    .erase = carbon_list_create(sizeof(u64))
  };
}

void carbon_slotmap_destroy(CBN_SlotMap *sm) {
  carbon_list_destroy(&sm->indices);
  carbon_list_destroy(&sm->data);
  carbon_list_destroy(&sm->erase);
}

CBN_SlotMap_Key carbon_slotmap_push(CBN_SlotMap *sm, void *value) {
  u64 alloc_id = carbon_slotmap__alloc(sm);
  CBN_SlotMap_Key *slot = &carbon_list_at_raw(CBN_SlotMap_Key, sm->indices, alloc_id);
  if (slot->id >= sm->data.size) carbon_list_push(&sm->data, value);
  else  carbon_memory_copy((void *) ((u64) sm->data.items + (slot->id * sm->stride)), value, sm->stride);
  if (slot->id >= sm->erase.size) carbon_list_push(&sm->erase, &alloc_id);
  else carbon_list_at_raw(u64, sm->erase, slot->id) = alloc_id;
  CBN_SlotMap_Key key = *slot;
  key.id = alloc_id;
  return key;
}

u8 carbon_slotmap_remove(CBN_SlotMap *sm, const CBN_SlotMap_Key key) {
  if (!carbon_slotmap__is_valid_key(sm, key)) return false;
  carbon_slotmap__free(sm, key);
  return true;
}

u8 carbon_slotmap_lookup(const CBN_SlotMap *sm, const CBN_SlotMap_Key key, void *out_value) {
  if (!carbon_slotmap__is_valid_key(sm, key)) return false;
  u64 idx = carbon_list_at(CBN_SlotMap_Key, sm->indices, key.id).id;
  carbon_memory_copy(out_value, (void *) ((u64) sm->data.items + (idx * sm->stride)), sm->stride);
  return true;
}

char *carbon_slotmap_key_to_cstr(const CBN_SlotMap_Key key) {
  return carbon_string_fmt("(%llu, %llu)", key.id, key.gen);
}

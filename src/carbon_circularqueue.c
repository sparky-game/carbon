// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_CircularQueue carbon_circularqueue_create(usz capacity, usz stride) {
  return (CBN_CircularQueue) {
    .items = carbon_memory_alloc(capacity * stride),
    .capacity = capacity,
    .stride = stride,
    .size = 0,
    .head = 0,
    .tail = -1
  };
}

void carbon_circularqueue_destroy(CBN_CircularQueue *cq) {
  if (!cq) {
    CBN_WARN("`cq` is not a valid pointer, skipping destruction");
    return;
  }
  carbon_memory_free(cq->items);
  carbon_memory_set(cq, 0, sizeof(*cq));
}

void carbon_circularqueue_push(CBN_CircularQueue *cq, void *value) {
  if (!cq || !value) {
    CBN_ERROR("`cq` and `value` must be valid pointers");
    return;
  }
  cq->tail = (cq->tail + 1) % cq->capacity;
  carbon_memory_copy((void *) ((u64) cq->items + (cq->tail * cq->stride)), value, cq->stride);
  cq->size = (cq->size + 1) % cq->capacity;
}

void carbon_circularqueue_pop(CBN_CircularQueue *cq, void *out_value) {
  carbon_circularqueue_peek(cq, out_value);
  cq->head = (cq->head + 1) % cq->capacity;
  --cq->size;
}

void carbon_circularqueue_peek(const CBN_CircularQueue *cq, void *out_value) {
  if (!cq || !out_value) {
    CBN_ERROR("`cq` and `out_value` must be valid pointers");
    return;
  }
  if (!cq->size) {
    CBN_WARN("circular queue is empty");
    return;
  }
  carbon_memory_copy(out_value, (void *) ((u64) cq->items + (cq->head * cq->stride)), cq->stride);
}

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "../carbon.h.in"
#include "carbon_deps.h"
#include "carbon_defs.h"
#include "carbon_types.h"
#include "carbon_log.h"
#include "carbon_string.h"
#include "carbon_assert.h"
#include "carbon_memory.h"
#include "carbon_circularqueue.h"

CBN_CircularQueue carbon_circularqueue_create(usz capacity, usz stride) {
  void *ptr = CARBON_MALLOC(capacity * stride);
  CARBON_ASSERT(ptr && "failed to allocate memory");
  return (CBN_CircularQueue) {
    .items = ptr,
    .capacity = capacity,
    .stride = stride,
    .size = 0,
    .head = 0,
    .tail = -1
  };
}

void carbon_circularqueue_destroy(CBN_CircularQueue *cq) {
  if (!cq) {
    carbon_log_warn("`cq` is not a valid pointer, skipping destruction");
    return;
  }
  if (cq->items) CARBON_FREE(cq->items);
  carbon_memory_set(cq, 0, sizeof(*cq));
}

void carbon_circularqueue_push(CBN_CircularQueue *cq, void *value) {
  if (!cq || !value) {
    carbon_log_error("`cq` and `value` must be valid pointers");
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
    carbon_log_error("`cq` and `out_value` must be valid pointers");
    return;
  }
  if (!cq->size) {
    carbon_log_warn("circular queue is empty");
    return;
  }
  carbon_memory_copy(out_value, (void *) ((u64) cq->items + (cq->head * cq->stride)), cq->stride);
}

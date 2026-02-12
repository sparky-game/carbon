/*
  $$===========================$$
  ||       CircularQueue       ||
  $$===========================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a circular queue; given a fixed size, fills to capacity and then overwrites earliest item.
 */
CBNDEF_TT(CBN_CircularQueue) {
  void *items;
  usz capacity;
  usz stride;
  usz size;
  i32 head, tail;
};
CBNDEF_TAKA(cbn, CircularQueue, CBN_CircularQueue);

#define carbon_circularqueue_at_raw(T, cq, i) ((T *) (cq).items)[(i)]
#define carbon_circularqueue_at(T, cq, i) (CBN_ASSERT((i) < (cq).size && "CircularQueue index out of bounds"), CBN_ASSERT(sizeof(T) == (cq).stride && "CircularQueue type doesn't match"), carbon_circularqueue_at_raw(T, cq, i))
#define carbon_circularqueue_foreach(T, cq) for (struct { usz i; T var; } it = {0, carbon_circularqueue_at(T, cq, 0)}; it.i < (cq).size; ++it.i, it.i < (cq).size ? it.var = carbon_circularqueue_at(T, cq, it.i) : it.var)

/**
 * @brief Create a new CircularQueue/ring container.
 * @param capacity Number of elements it will hold.
 * @param stride Size in bytes of the elements it will hold.
 * @return The CircularQueue/ring container.
 */
CBNDEF CBN_CircularQueue carbon_circularqueue_create(usz capacity, usz stride);

/**
 * @brief Destroy a CircularQueue/ring container.
 * @param cq The CircularQueue/ring container.
 */
CBNDEF void carbon_circularqueue_destroy(CBN_CircularQueue *cq);

/**
 * @brief Appends a copy of the value to the end of the CircularQueue/ring.
 * @param cq The CircularQueue/ring container.
 * @param value The value to append.
 */
CBNDEF void carbon_circularqueue_push(CBN_CircularQueue *cq, void *value);

/**
 * @brief Removes the first element from the CircularQueue/ring (FIFO).
 * @param cq The CircularQueue/ring container.
 * @param out_value The value of the element popped out.
 */
CBNDEF void carbon_circularqueue_pop(CBN_CircularQueue *cq, void *out_value);

/**
 * @brief Gets the first element from the CircularQueue/ring (FIFO) as if it were actually popped out.
 * @param cq The CircularQueue/ring container.
 * @param out_value The value of the element peeked at.
 */
CBNDEF void carbon_circularqueue_peek(const CBN_CircularQueue *cq, void *out_value);

#ifdef __cplusplus
template <typename T>
struct CBN_CircularQueue_tt : CBN_CircularQueue_t {
  using value_type = T;
  using iterator = T *;

  static CBN_CircularQueue_tt make(usz capacity) {
    auto cq = carbon_circularqueue_create(capacity, sizeof(value_type));
    return *(CBN_CircularQueue_tt *) &cq;
  }

  void Free(void) {
    carbon_circularqueue_destroy((CBN_CircularQueue *) this);
  }

  void Push(const value_type &value) {
    carbon_circularqueue_push((CBN_CircularQueue *) this, (void *) &value);
  }
  
  value_type Pop(void) {
    value_type x;
    carbon_circularqueue_pop((CBN_CircularQueue *) this, &x);
    return x;
  }

  value_type Peek(void) {
    value_type x;
    carbon_circularqueue_peek((CBN_CircularQueue *) this, &x);
    return x;
  }

  iterator begin(void) const {
    return (iterator) items;
  }

  iterator end(void) const {
    return (iterator) items + size;
  }

  value_type &operator[](usz idx) {
    return carbon_circularqueue_at(value_type, *this, idx);
  }
};
#endif

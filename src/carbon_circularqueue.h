/*
**  $$===========================$$
**  ||       CircularQueue       ||
**  $$===========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Represents a circular buffer/queue; given a fixed size, fills to capacity and then overwrites earliest item.
 *
 * In C++, this is a templated class/struct, which means it's not a type by itself,
 * until it gets instantiated with the needed template arguments. The type `CBN_CircularQueue`
 * is an alias for `CBN_CircularQueue_t<void *>`, which provides full-compatibility with the
 * C API, even through C++ templates.
 *
 * @param T Type information of what will be stored in the container.
 */
#ifdef __cplusplus
template <typename T>
#endif
struct CBN_CircularQueue_t {
  void *items;
  usz capacity;
  usz stride;
  usz size;
  i32 head, tail;
#ifdef __cplusplus
  using value_type = T;
  using iterator = T *;
  /**
   * @brief carbon_circularqueue_create
   * @param capacity Number of elements it will hold.
   * @return The CircularQueue/ring container.
   */
  static CBN_CircularQueue_t make(usz capacity);
  /**
   * @brief carbon_circularqueue_destroy
   */
  void Free(void);
  /**
   * @brief carbon_circularqueue_push
   * @param value The value to append.
   */
  void Push(const value_type &value);
  /**
   * @brief carbon_circularqueue_pop
   * @return The value of the element popped out.
   */
  value_type Pop(void);
  /**
   * @brief carbon_circularqueue_peek
   * @return The value of the element peeked at.
   */
  value_type Peek(void);
  /**
   * @brief Returns an iterator to the beginning.
   * @return The iterator.
   */
  iterator begin(void) const;
  /**
   * @brief Returns an iterator to the end.
   * @return The iterator.
   */
  iterator end(void) const;
  // Overloaded Operators
  value_type &operator[](usz idx);
  const value_type &operator[](usz idx) const;
#endif
};
#ifdef __cplusplus
using CBN_CircularQueue = CBN_CircularQueue_t<void *>;
#else
typedef struct CBN_CircularQueue_t CBN_CircularQueue;
#endif

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

// Local Variables:
// mode: c++
// End:

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifdef __cplusplus

template <typename T>
CBN_CircularQueue_t<T> CBN_CircularQueue_t<T>::make(usz capacity) {
  auto cq = carbon_circularqueue_create(capacity, sizeof(T));
  return *(CBN_CircularQueue_t<T> *) &cq;
}

template <typename T>
void CBN_CircularQueue_t<T>::Free(void) {
  carbon_circularqueue_destroy((CBN_CircularQueue *) this);
}

template <typename T>
void CBN_CircularQueue_t<T>::Push(const value_type &value) {
  carbon_circularqueue_push((CBN_CircularQueue *) this, (void *) &value);
}

template <typename T>
typename CBN_CircularQueue_t<T>::value_type CBN_CircularQueue_t<T>::Pop(void) {
  value_type x;
  carbon_circularqueue_pop((CBN_CircularQueue *) this, &x);
  return x;
}

template <typename T>
typename CBN_CircularQueue_t<T>::value_type CBN_CircularQueue_t<T>::Peek(void) {
  value_type x;
  carbon_circularqueue_peek((CBN_CircularQueue *) this, &x);
  return x;
}

template <typename T>
typename CBN_CircularQueue_t<T>::iterator CBN_CircularQueue_t<T>::begin(void) const {
  return (iterator) items;
}

template <typename T>
typename CBN_CircularQueue_t<T>::iterator CBN_CircularQueue_t<T>::end(void) const {
  return (iterator) items + size;
}

template <typename T>
typename CBN_CircularQueue_t<T>::value_type &CBN_CircularQueue_t<T>::operator[](usz idx) {
  return const_cast<value_type &>(static_cast<const CBN_CircularQueue_t &>(*this)[idx]);
}

template <typename T>
const typename CBN_CircularQueue_t<T>::value_type &CBN_CircularQueue_t<T>::operator[](usz idx) const {
  CBN_ASSERT(idx < size && "CircularQueue index out of bounds");
  CBN_ASSERT(sizeof(value_type) == stride && "CircularQueue type doesn't match");
  return ((value_type *) items)[idx];
}

#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:

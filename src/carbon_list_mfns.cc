// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __cplusplus

template <typename T>
CBN_List_t<T> CBN_List_t<T>::make(void) {
  auto l = carbon_list_create(sizeof(T));
  return *(CBN_List_t<T> *) &l;
}

template <typename T>
void CBN_List_t<T>::Free(void) {
  carbon_list_destroy((CBN_List *) this);
}

template <typename T>
void CBN_List_t<T>::Push(const T &value) {
  carbon_list_push((CBN_List *) this, (void *) &value);
}

template <typename T>
T CBN_List_t<T>::Pop(void) {
  T x;
  carbon_list_pop(this, &x);
  return x;
}

template <typename T>
isz CBN_List_t<T>::Find(const T &value) const {
  return carbon_list_find(this, value);
}

template <typename T>
void CBN_List_t<T>::Remove(usz idx) {
  carbon_list_remove(this, idx);
}

template <typename T>
typename CBN_List_t<T>::iterator CBN_List_t<T>::begin(void) {
  return const_cast<iterator>(static_cast<const CBN_List_t &>(*this).cbegin());
}

template <typename T>
typename CBN_List_t<T>::const_iterator CBN_List_t<T>::cbegin(void) const {
  return static_cast<const_iterator>(items);
}

template <typename T>
typename CBN_List_t<T>::iterator CBN_List_t<T>::end(void) {
  return const_cast<iterator>(static_cast<const CBN_List_t &>(*this).cend());
}

template <typename T>
typename CBN_List_t<T>::const_iterator CBN_List_t<T>::cend(void) const {
  return static_cast<const_iterator>(items) + size;
}

template <typename T>
T &CBN_List_t<T>::operator[](usz idx) {
  return const_cast<T &>(static_cast<const CBN_List_t &>(*this)[idx]);
}

template <typename T>
const T &CBN_List_t<T>::operator[](usz idx) const {
  CARBON_ASSERT(0 <= idx && idx < size && "List index out of bounds");
  CARBON_ASSERT(sizeof(T) == stride && "List type doesn't match");
  return ((T *) items)[idx];
}

#endif  // __cplusplus

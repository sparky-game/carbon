// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifdef __cplusplus

template <typename T>
CBN_TList<T> CBN_TList<T>::make(void) {
  auto l = carbon_list_create(sizeof(T));
  return *(CBN_TList<T> *) &l;
}

template <typename T>
void CBN_TList<T>::Free(void) {
  carbon_list_destroy((CBN_List *) this);
}

template <typename T>
void CBN_TList<T>::Push(const value_type &value) {
  carbon_list_push((CBN_List *) this, (void *) &value);
}

template <typename T>
typename CBN_TList<T>::value_type CBN_TList<T>::PopFront(void) {
  value_type x;
  carbon_list_pop_front((CBN_List *) this, &x);
  return x;
}

template <typename T>
typename CBN_TList<T>::value_type CBN_TList<T>::PopBack(void) {
  value_type x;
  carbon_list_pop_back((CBN_List *) this, &x);
  return x;
}

template <typename T>
isz CBN_TList<T>::Find(const value_type &value) const {
  return carbon_list_find((CBN_List *) this, (void *) &value);
}

template <typename T>
void CBN_TList<T>::Remove(usz idx) {
  carbon_list_remove((CBN_List *) this, idx);
}

template <typename T>
void CBN_TList<T>::ShrinkToFit(void) {
  carbon_list_shrink_to_fit((CBN_List *) this);
}

template <typename T>
typename CBN_TList<T>::iterator CBN_TList<T>::begin(void) const {
  return (iterator) items;
}

template <typename T>
typename CBN_TList<T>::iterator CBN_TList<T>::end(void) const {
  return (iterator) items + size;
}

template <typename T>
typename CBN_TList<T>::value_type &CBN_TList<T>::operator[](usz idx) {
  return carbon_list_at(value_type, *this, idx);
}

#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:

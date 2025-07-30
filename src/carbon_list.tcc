// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

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
void CBN_List_t<T>::Push(const value_type &value) {
  carbon_list_push((CBN_List *) this, (void *) &value);
}

template <typename T>
typename CBN_List_t<T>::value_type CBN_List_t<T>::Pop(void) {
  value_type x;
  carbon_list_pop((CBN_List *) this, &x);
  return x;
}

template <typename T>
isz CBN_List_t<T>::Find(const value_type &value) const {
  return carbon_list_find((CBN_List *) this, (void *) &value);
}

template <typename T>
void CBN_List_t<T>::Remove(usz idx) {
  carbon_list_remove((CBN_List *) this, idx);
}

template <typename T>
void CBN_List_t<T>::ShrinkToFit(void) {
  carbon_list_shrink_to_fit((CBN_List *) this);
}

template <typename T>
typename CBN_List_t<T>::iterator CBN_List_t<T>::begin(void) const {
  return (iterator) items;
}

template <typename T>
typename CBN_List_t<T>::iterator CBN_List_t<T>::end(void) const {
  return (iterator) items + size;
}

template <typename T>
typename CBN_List_t<T>::value_type &CBN_List_t<T>::operator[](usz idx) {
  return const_cast<value_type &>(static_cast<const CBN_List_t &>(*this)[idx]);
}

template <typename T>
const typename CBN_List_t<T>::value_type &CBN_List_t<T>::operator[](usz idx) const {
  CBN_ASSERT(idx < size && "List index out of bounds");
  CBN_ASSERT(sizeof(value_type) == stride && "List type doesn't match");
  return ((value_type *) items)[idx];
}

#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifdef __cplusplus

template <typename T>
CBN_SlotMap_t<T> CBN_SlotMap_t<T>::make(void) {
  auto sm = carbon_slotmap_create(sizeof(T));
  return *(CBN_SlotMap_t<T> *) &sm;
}

template <typename T>
void CBN_SlotMap_t<T>::Free(void) {
  carbon_slotmap_destroy((CBN_SlotMap *) this);
}

template <typename T>
CBN_SlotMap_Key CBN_SlotMap_t<T>::Push(const value_type &value) {
  return carbon_slotmap_push((CBN_SlotMap *) this, (void *) &value);
}

template <typename T>
u8 CBN_SlotMap_t<T>::Remove(const CBN_SlotMap_Key key) {
  return carbon_slotmap_remove((CBN_SlotMap *) this, key);
}

template <typename T>
typename CBN_SlotMap_t<T>::iterator CBN_SlotMap_t<T>::begin(void) const {
  return ((data_type *) &data)->begin();
}

template <typename T>
typename CBN_SlotMap_t<T>::iterator CBN_SlotMap_t<T>::end(void) const {
  return ((data_type *) &data)->end();
}

template <typename T>
cbn::Opt<typename CBN_SlotMap_t<T>::value_type> CBN_SlotMap_t<T>::operator[](const CBN_SlotMap_Key key) {
  return static_cast<const CBN_SlotMap_t &>(*this)[key];
}

template <typename T>
cbn::Opt<const typename CBN_SlotMap_t<T>::value_type> CBN_SlotMap_t<T>::operator[](const CBN_SlotMap_Key key) const {
  value_type x;
  if (!carbon_slotmap_lookup((CBN_SlotMap *) this, key, &x)) return {};
  return x;
}

#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:

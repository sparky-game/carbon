// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __cplusplus

CBN_List::iterator CBN_List::begin(void) {
  return items;
}

CBN_List::const_iterator CBN_List::begin(void) const {
  return items;
}

CBN_List::const_iterator CBN_List::cbegin(void) const {
  return items;
}

CBN_List::iterator CBN_List::end(void) {
  return (iterator) ((u64) items + (size * stride));
}

CBN_List::const_iterator CBN_List::end(void) const {
  return (const_iterator) ((u64) items + (size * stride));
}

CBN_List::const_iterator CBN_List::cend(void) const {
  return (const_iterator) ((u64) items + (size * stride));
}

void CBN_List::Push(void *value) {
  carbon_list_push(this, value);
}

void CBN_List::Pop(void *out_value) {
  carbon_list_pop(this, out_value);
}

isz CBN_List::Find(const void *value) const {
  return carbon_list_find(this, value);
}

void CBN_List::Remove(usz idx) {
  carbon_list_remove(this, idx);
}

#endif  // __cplusplus

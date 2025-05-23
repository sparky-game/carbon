// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __cplusplus

CBN_StrList CBN_StrList::make(void) {
  return carbon_strlist_create(false);
}

const CBN_StrList CBN_StrList::make(const char *s, const char *delim) {
  return carbon_strlist_from_splitted_cstr(s, delim);
}

CBN_StrList CBN_StrList::make_unique(void) {
  return carbon_strlist_create(true);
}

void CBN_StrList::Push(const char *s) {
  carbon_strlist_push(this, s);
}

CBN_StrList::iterator CBN_StrList::begin(void) {
  return const_cast<iterator>(static_cast<const CBN_StrList &>(*this).begin());
}

CBN_StrList::const_iterator CBN_StrList::begin(void) const {
  return items;
}

CBN_StrList::iterator CBN_StrList::end(void) {
  return const_cast<iterator>(static_cast<const CBN_StrList &>(*this).end());
}

CBN_StrList::const_iterator CBN_StrList::end(void) const {
  return items + size;
}

#endif  // __cplusplus

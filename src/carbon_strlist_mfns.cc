// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __cplusplus

CBN_StrList::iterator CBN_StrList::begin(void) {
  return (iterator) items;
}

CBN_StrList::const_iterator CBN_StrList::begin(void) const {
  return items;
}

CBN_StrList::const_iterator CBN_StrList::cbegin(void) const {
  return items;
}

CBN_StrList::iterator CBN_StrList::end(void) {
  return (iterator) items + size;
}

CBN_StrList::const_iterator CBN_StrList::end(void) const {
  return items + size;
}

CBN_StrList::const_iterator CBN_StrList::cend(void) const {
  return items + size;
}

#endif  // __cplusplus

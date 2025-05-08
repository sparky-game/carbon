// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __cplusplus

CBN_PatternMatchedFiles::iterator CBN_PatternMatchedFiles::begin(void) {
  return (CBN_PatternMatchedFiles::iterator) files;
}

CBN_PatternMatchedFiles::const_iterator CBN_PatternMatchedFiles::begin(void) const {
  return files;
}

CBN_PatternMatchedFiles::const_iterator CBN_PatternMatchedFiles::cbegin(void) const {
  return files;
}

CBN_PatternMatchedFiles::iterator CBN_PatternMatchedFiles::end(void) {
  return (CBN_PatternMatchedFiles::iterator) (files + count);
}

CBN_PatternMatchedFiles::const_iterator CBN_PatternMatchedFiles::end(void) const {
  return files + count;
}

CBN_PatternMatchedFiles::const_iterator CBN_PatternMatchedFiles::cend(void) const {
  return files + count;
}

#endif  // __cplusplus

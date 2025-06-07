// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

void *carbon_memory_copy(void *dst, const void *src, usz n) {
  u8 *d = (u8 *) dst;
  const u8 *s = (const u8 *) src;
  for (; n; --n) *d++ = *s++;
  return dst;
}

i32 carbon_memory_cmp(const void *v1, const void *v2, usz n) {
  const u8 *l = v1, *r = v2;
  for (; n && *l == *r; --n, ++l, ++r);
  return n ? *l - *r : 0;
}

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

void *carbon_memory_copy(void *dst, const void *src, usz n) {
  u8 *d = (u8 *) dst;
  const u8 *s = (const u8 *) src;
  while (n--) *d++ = *s++;
  return dst;
}

i32 carbon_memory_cmp(const void *v1, const void *v2, usz n) {
  const u8 *l = (const u8 *) v1, *r = (const u8 *) v2;
  for (; n && *l == *r; --n, ++l, ++r);
  return n ? *l - *r : 0;
}

void *carbon_memory_set(void *dst, i32 c, usz n) {
  u8 *d = (u8 *) dst;
  while (n--) *d++ = c;
  return dst;
}

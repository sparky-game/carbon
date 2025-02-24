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

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

void *carbon_memory_alloc(usz size) {
  void *p = __builtin_malloc(size);
  CBN_ASSERT(p && "failed to allocate memory");
  return p;
}

void *carbon_memory_zeroed(usz size) {
  void *p = __builtin_calloc(1, size);
  CBN_ASSERT(p && "failed to allocate memory");
  return p;
}

void *carbon_memory_realloc(void *p, usz size) {
  void *new_p = __builtin_realloc(p, size);
  CBN_ASSERT(new_p && "failed to reallocate memory");
  return new_p;
}

void *carbon_memory_copy(void * restrict dst, const void * restrict src, usz n) {
#if CARBON_HAS_BUILTIN(__builtin_memcpy)
  return __builtin_memcpy(dst, src, n);
#else
#warning `__builtin_memcpy` not available, using alternative slower implementation
  u8 *d = (u8 *) dst;
  const u8 *s = (const u8 *) src;
  while (n--) *d++ = *s++;
  return dst;
#endif
}

i32 carbon_memory_cmp(const void *v1, const void *v2, usz n) {
#if CARBON_HAS_BUILTIN(__builtin_memcmp)
  return __builtin_memcmp(v1, v2, n);
#else
#warning `__builtin_memcmp` not available, using alternative slower implementation
  const u8 *l = (const u8 *) v1, *r = (const u8 *) v2;
  for (; n && *l == *r; --n, ++l, ++r);
  return n ? *l - *r : 0;
#endif
}

void *carbon_memory_set(void *dst, i32 c, usz n) {
#if CARBON_HAS_BUILTIN(__builtin_memset)
  return __builtin_memset(dst, c, n);
#else
#warning `__builtin_memset` not available, using alternative slower implementation
  u8 *d = (u8 *) dst;
  while (n--) *d++ = (u8) c;
  return dst;
#endif
}

void carbon_memory_free(void *p) {
  if (!p) return;
  __builtin_free(p);
}
